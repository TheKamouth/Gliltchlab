#include "glilcthlabcore.h"

#include <QDebug>

void GetPixel(int x, int y, std::vector< QRgb>* line, QPixmap *inputImage, QImage *outputImage, int index){

    //std::cout << "(x, y): "<< x << " " << y << std::endl;

    QRgb tmp = inputImage->toImage().pixel( x, y);

    line->push_back( tmp);
}

void DrawPixel(int x, int y, std::vector< QRgb>* line, QPixmap *inputImage, QImage *outputImage, int index){

    outputImage->setPixel( x, y, line->at( index));
}

GlilcthLabCore::GlilcthLabCore() : abortProcess(false), areResourcesAllocated(false)
{

    Init();
}

void GlilcthLabCore::Init(){

    getPixel = &GetPixel;
    drawPixel = &DrawPixel;

    glitchParameters = new GlitchParameters();

    myCompClass.rule = glitchParameters->sortingRule;
}

void GlilcthLabCore::SetInputImage(QString fileName){

    if( areResourcesAllocated){
        ClearResources();
    }

    inputImage = new QPixmap(fileName);           

    outputQImage = new QImage(inputImage->size(), QImage::Format_RGB32);
    areResourcesAllocated = true;
}

void GlilcthLabCore::SetInputPixmap(QPixmap &pixmap){    

    if( areResourcesAllocated){
        ClearResources();
    }

    if(pixmap.isNull())
        qDebug() << __FUNCTION__<<" pixmap is null";

    inputImage = &pixmap;

    outputQImage = new QImage(inputImage->size(), QImage::Format_RGB32);
    areResourcesAllocated = true;
}

//IMAGE PROCESSING
void GlilcthLabCore::ProcessGlitch(){

    if(inputImage->isNull()){
        std::cout<< __FUNCTION__<<" input image is null" <<std::endl;
        return;
    }   

    QImage inputQImage = inputImage->toImage();

    abortProcess = false;

    int sortingAngle = (int) glitchParameters->sortingAngle;
    bool isCircularSorting = glitchParameters->isCircularSorting;
    bool isSegmentationEnabled = glitchParameters->segmentationRule->IsSegmentationEnabled();

    int threshold = glitchParameters->segmentationRule->GetThreshold();
    if (glitchParameters->sortingRule->currentRule == SortingRule::HueOnly)
        threshold *= 3.59;
    else
        threshold *= 2.55;

    QImage* maskImage = glitchParameters->mask.GetMask();

    if( (glitchParameters->mask.GetMaskType() != 0) && (glitchParameters->mask.GetMask()->isNull())){
        std::cout<< "no mask loaded" <<std::endl;
        return;
    }

    myCompClass.isInverted = glitchParameters->invertSortingRule;

    //qDebug() << "sorting angle: "<<sortingAngle;

    if( isCircularSorting){

        qDebug() << glitchParameters->sortingCenter.x() << "  "<<(double)inputImage->width() << " " << (double)inputImage->width() * glitchParameters->sortingCenter.x();

        //QPoint imageCenter = QPoint( inputImage->width() / 2, inputImage->height() / 2);
        QPoint imageCenter = QPoint( (double)inputImage->width() * glitchParameters->sortingCenter.x(), (double)inputImage->height() * glitchParameters->sortingCenter.y());
        QPoint p1;

        //used to keep track of progress
        int maxOperations = 2 * inputImage->width() + 2 * inputImage->height();
        int operationCount = 0;

        //top pixel line
        for( int x = 0; x < inputImage->width(); x++){

            std::vector< QRgb>* line = new std::vector< QRgb>();
            line->reserve( 1024);

            //deal with process cancel
            if( abortProcess){

                delete line;
                return;
            }

            //updating progressBar
            operationCount++;
            int progressValue = 100 * operationCount / maxOperations;
            SendProcessProgress( progressValue);            

            p1 = QPoint( x, 0);            

            //get line
            Bresenham(imageCenter, p1, line, getPixel);

            //sort line
            std::sort ( line->begin(), line->end(), myCompClass);

            //draw sorted line
            Bresenham( imageCenter, p1, line, drawPixel);

            delete line;
        }

        //bottom pixel line
        for( int x = 0; x < inputImage->width(); x++){

            std::vector< QRgb>* line = new std::vector< QRgb>();
            line->reserve( 1024);

            //deal with process cancel
            if( abortProcess){

                delete line;
                return;
            }

            //updating progressBar
            operationCount++;
            int progressValue = 100 * operationCount / maxOperations;
            SendProcessProgress( progressValue);

            p1 = QPoint( x, inputImage->height() - 1);

            //get line
            Bresenham(imageCenter, p1, line, getPixel);

            //sort line
            std::sort ( line->begin(), line->end(), myCompClass);

            //draw sorted line
            Bresenham( imageCenter, p1, line, drawPixel);

            delete line;
        }

        qDebug() << "left column";
        //left pixel column
        for( int y = 0; y < inputImage->height(); y++){

            std::vector< QRgb>* line = new std::vector< QRgb>();
            line->reserve( 1024);

            //deal with process cancel
            if( abortProcess){

                delete line;
                return;
            }

            //updating progressBar
            operationCount++;
            int progressValue = 100 * operationCount / maxOperations;
            SendProcessProgress( progressValue);

            p1 = QPoint( 0, y);

            //get line
            Bresenham(imageCenter, p1, line, getPixel);

            //sort line
            std::sort ( line->begin(), line->end(), myCompClass);

            //draw sorted line
            Bresenham( imageCenter, p1, line, drawPixel);

            delete line;
        }

        qDebug() << "right pixel column";
        //right pixel column
        for( int y = 0; y < inputImage->height(); y++){

            std::vector< QRgb>* line = new std::vector< QRgb>();
            line->reserve( 1024);

            //deal with process cancel
            if( abortProcess){

                delete line;
                return;
            }

            //updating progressBar
            operationCount++;
            int progressValue = 100 * operationCount / maxOperations;
            SendProcessProgress( progressValue);

            p1 = QPoint( inputImage->width() - 1, y);

            //std::cout << "drawing (" << imageCenter.x() << ", " << imageCenter.y() << " ) , (" << p1.x() << ", " << p1.y() << " )" <<std::endl;

            //get line
            Bresenham(imageCenter, p1, line, getPixel);

            //std::cout << "line->size(): " << line->size() << std::endl;

            //sort line
            std::sort ( line->begin(), line->end(), myCompClass);

            //draw sorted line
            Bresenham( imageCenter, p1, line, drawPixel);

            delete line;
        }

    }
    else{
        if(std::abs( sortingAngle) == 90){

            for(int x = 0; x < inputImage->size().width(); x++){

                //std::map< int, QRgb>* line = new std::map< int, QRgb>();
                std::vector< QRgb>* line = new std::vector< QRgb>();
                line->reserve(1024);

                if( abortProcess){
                    delete line;
                    return;
                }

                int progressValue = 100 * x / inputImage->size().width();
                SendProcessProgress( progressValue);                

                QPoint p0 = QPoint(0, 0);   //point min dans le repere de la droite d'angle sortingAngle passant par p(x, 0)
                QPoint p1 = QPoint(0, 0);   //point max ...

                QPoint currentP0 = QPoint(x, 0);
                QPoint currentP1 = QPoint(x, 1);

                if (isSegmentationEnabled == false || threshold <= 0.0)
                    currentP1 = QPoint(x, inputImage->height() - 1);

                QColor p0Color = QColor();
                QColor p1Color = QColor();
                QColor previousP1Color = QColor();
                bool columnProcessed = false;

                //on traite la colonne entiere
                while( !columnProcessed){

                    //On trouve p0 et p1 de la sous colonne a traiter
                    p0Color = inputQImage.pixel( currentP0);
                    p1Color = inputQImage.pixel( currentP1);

                    //function diff compris entre 0 et 1 (cas de la hue géré plus bas)
                    float functionDiff = abs(glitchParameters->sortingRule->GetColorValue(p0Color)-glitchParameters->sortingRule->GetColorValue(p1Color));

                    while( functionDiff < threshold && (currentP1.y() < inputImage->height() - 1) ){

                        previousP1Color = inputQImage.pixel( currentP1);
                        currentP1 += QPoint(0, 1);
                        p1Color = inputQImage.pixel( currentP1);

                        functionDiff = abs( glitchParameters->sortingRule->GetColorValue( p0Color) - glitchParameters->sortingRule->GetColorValue( p1Color));
                        //functionDiff = abs( glitchParameters->sortingRule->GetColorValue(previousP1Color) - glitchParameters->sortingRule->GetColorValue( p1Color));
                    }

                    if(currentP1.y() >= inputImage->height() - 1){
                        columnProcessed = true;
                    }

                    p0 = currentP0;
                    p1 = currentP1;

                    //get line
                    Bresenham(p0, p1, line, getPixel);

                    //Sort line
                    std::sort ( line->begin(), line->end(), myCompClass);

                    //draw sorted line
                    Bresenham( p0, p1, line, drawPixel);

                    currentP0 = currentP1 + QPoint( 0, 1);
                    currentP1 += QPoint(0, 1);

                    line->clear();
                }
                delete line;
            }
        }
        else if( ( std::abs( sortingAngle) == 180) || ( sortingAngle == 0.0) ){

            for(int y = 0; y < inputImage->size().height(); y++){

                std::vector< QRgb>* line = new std::vector< QRgb>();
                line->reserve(1024);

                if( abortProcess){
                    delete line;
                    return;
                }

                int progressValue = 100 * y / inputImage->size().height();
                SendProcessProgress( progressValue);

                QPoint p0 = QPoint(0, 0);   //point min dans le repere de la droite d'angle sortingAngle passant par p(x, 0)
                QPoint p1 = QPoint(0, 0);   //point max ...

                QPoint currentP0 = QPoint(0, y);
                QPoint currentP1 = QPoint(1, y);
                if (isSegmentationEnabled == false)
                    currentP1 = QPoint(inputImage->width() - 1, y);

                QColor p0Color = QColor();
                QColor p1Color = QColor();
                bool columnProcessed = false;

                while( !columnProcessed){

                    //On trouve p0 et p1 de la sous colonne a traiter
                    p0Color = inputQImage.pixel( currentP0);
                    p1Color = inputQImage.pixel( currentP1);

                    //function diff compris entre 0 et 1 (cas de la hue géré plus bas)
                    float functionDiff = abs(glitchParameters->sortingRule->GetColorValue(p0Color)-glitchParameters->sortingRule->GetColorValue(p1Color));

                    while( functionDiff < threshold && (currentP1.x() < inputImage->width() - 1) ){

                        currentP1 += QPoint(1, 0);
                        p1Color = inputQImage.pixel( currentP1);

                        functionDiff = abs(glitchParameters->sortingRule->GetColorValue(p0Color)-glitchParameters->sortingRule->GetColorValue(p1Color));
                    }

                    if(currentP1.x() >= inputImage->width() - 1){
                        columnProcessed = true;
                    }

                    p0 = currentP0;
                    p1 = currentP1;

                    //get line
                    Bresenham(p0, p1, line, getPixel);

                    //Sort line
                    std::sort ( line->begin(), line->end(), myCompClass);

                    //draw sorted line
                    Bresenham( p0, p1, line, drawPixel);

                    currentP0 = currentP1 + QPoint(1, 0);
                    currentP1 += QPoint(1, 0);

                    line->clear();
                }
                delete line;
            }

        }
        //TODOb
        else{
            return ProcessGlitchRefactor();  //TODO
        }

    }

    //testing bresenham implementation
    //DrawTestLine();

    SendProcessGlitchDone();
}

void GlilcthLabCore::ClearResources(){

    //delete inputImage;

    //delete outputQImage;
}

void GlilcthLabCore::ProcessGlitchRefactor(){

    QPoint p0 = QPoint();
    QPoint p1 = QPoint();

    int maxOperations = inputImage->width() + inputImage->height();
    int operationCount = 0;

    float tanSortingAngle = tan( (double)(glitchParameters->sortingAngle * 3.14 / 180.0) );
    int yDiff = tanSortingAngle * (inputImage->width() - 1);

    for( int y = 0 ; y < inputImage->height() ; y ++){
    //for( int y = 0 ; y < 20 ; y ++){

        if( abortProcess){
            return;
        }

        //updating progressBar
        operationCount++;
        int progressValue = 100 * operationCount / maxOperations;
        SendProcessProgress( progressValue);

        std::vector< QRgb>* line = new std::vector< QRgb>();
        line->reserve( 1024);

        int xEndLine = 0;   //used for "edge case"
        int yEndLine = y + yDiff;

        p0 = QPoint(0, y);

        if( yEndLine >= inputImage->height()){

            //SendProcessGlitchDone();
            //return;

            yEndLine = inputImage->height() - 1;

            xEndLine = ( inputImage->height() - 1 - y) / tanSortingAngle;

            p1 = QPoint( xEndLine, yEndLine);
        }
        else{

            p1 = QPoint( inputImage->width() - 1, yEndLine);
        }

        //get line
        Bresenham(p0, p1, line, getPixel);

        //sort line        
        std::sort ( line->begin(), line->end(), myCompClass);

        //draw sorted line        
        Bresenham( p0, p1, line, drawPixel);
    }

    /*
    for( int x = 0 ; x < inputImage->width() ; x ++){

        if( abortProcess){
            return;
        }

        std::vector< QRgb>* line = new std::vector< QRgb>();
        line->reserve( 1024);

        int yLimit = tan( (double)(glitchParameters->sortingAngle * 3.14 / 180.0) ) * (inputImage->height() - y);

        if( yLimit < 0){ yLimit = 0;}
        if( yLimit > inputImage->height()){ yLimit = inputImage->height() - 1;}

        p0 = QPoint(0, y);
        p1 = QPoint( xLimit, inputImage->height());

        //get line
        Bresenham(p0, p1, line, getPixel);

        //sort line
        std::sort ( line->begin(), line->end(), myCompClass);

        //draw sorted line
        Bresenham( p0, p1, line, drawPixel);
    }*/

    SendProcessGlitchDone();
}

void GlilcthLabCore::AbortProcess(){

    abortProcess = true;
}


void GlilcthLabCore::DrawLine(std::vector< QRgb> *line, QPoint p0, QPoint p1){

    int deltaX = p1.x() - p0.x();
    int deltaY = p1.y() - p0.y();

    int signX = (deltaX > 0 ?  1 : -1);
    int signY = (deltaY > 0 ?  1 : -1);
/*
    if( deltaY < 0){

        deltaY *= -1.0;
    }

    if( deltaX < 0){

        deltaX *= -1.0;
    }*/

    int D = 2 * deltaY - deltaX;

    int index = 0;

    QRgb tmpCol = line->at(index);
    //QRgb tmpCol = qRgb(255, 0, 0);

    outputQImage->setPixel( p0.x(), p0.y(), tmpCol);

    int y = p0.y();

    for( int x = p0.x() + signX; (signX > 0 ? x < p1.x() : x > p1.x() ); x += signX){

        if( D > 0 ){

            if( signY == -1){
                y --;   // 0 < angle < 90
            }
            else{
                y ++;
            }

            if( ( x < inputImage->width()) && ( x >= 0) && ( y < inputImage->height()) && ( y >= 0) ){

                //std::cout << "p: " << x << " " << y <<std::endl;

                if( index < line->size()){

                    index ++;

                    tmpCol = line->at( index);
                }
                else{

                    std::cout << " x out of range: " << index <<std::endl;

                    tmpCol = qRgb(255, 0, 0);
                    return;
                }

                if( ( GetOctantFromAngle( glitchParameters->sortingAngle) == 0) || ( GetOctantFromAngle( glitchParameters->sortingAngle) == 7) ){

                    outputQImage->setPixel( x, y, tmpCol);
                }
                else{
                    outputQImage->setPixel( y, x, tmpCol);
                }

                D += 2 * deltaY * signY - 2 * deltaX * signX;
            }
            else{

                return;
            }
        }
        else{

            if( ( x < inputImage->width()) && ( x >= 0) && ( y < inputImage->height()) && ( y >= 0) ){

                //std::cout << "p: " << x << " " << y <<std::endl;

                if( index < line->size()){

                    index ++;

                    tmpCol = line->at( index);
                }
                else{

                    std::cout << " x out of range: " << index <<std::endl;

                    tmpCol = qRgb(255, 0, 0);
                    return;
                }

                if( ( GetOctantFromAngle( glitchParameters->sortingAngle) == 0) || ( GetOctantFromAngle( glitchParameters->sortingAngle) == 7) ){

                    outputQImage->setPixel( x, y, tmpCol);
                }
                else{
                    outputQImage->setPixel( y, x, tmpCol);
                }

                D += 2 * deltaY * signY;
            }
            else{

                return;
            }
        }
    }
}

void GlilcthLabCore::DrawLine(std::vector< QRgb>::reverse_iterator lineIterator, QPoint p0, QPoint p1){

    int deltaX = p1.x() - p0.x();
    int deltaY = p1.y() - p0.y();

    int signX = (deltaX > 0 ?  1 : -1);
    int signY = (deltaY > 0 ?  1 : -1);

    if( deltaY < 0){

        deltaY *= -1.0;
    }

    if( deltaX < 0){

        deltaX *= -1.0;
    }

    int D = 2 * deltaY - deltaX;

    int index = 0;

    QRgb tmpCol = *lineIterator;
    lineIterator++;

    //QRgb tmpCol = qRgb(255, 0, 0);

    outputQImage->setPixel( p0.x(), p0.y(), tmpCol);

    int y = p0.y();

    for( int x = p0.x() + signX; (signX > 0 ? x < p1.x() : x > p1.x() ); x += signX){

        if( D > 0 ){

            if( signY == -1){
                y --;   // 0 < angle < 90
            }
            else{
                y ++;
            }

            if( ( x < inputImage->width()) && ( x >= 0) && ( y < inputImage->height()) && ( y >= 0) ){

                //std::cout << "p: " << x << " " << y <<std::endl;

                tmpCol = *lineIterator;

                lineIterator++;

                if( ( GetOctantFromAngle( glitchParameters->sortingAngle) == 0) || ( GetOctantFromAngle( glitchParameters->sortingAngle) == 7) ){

                    outputQImage->setPixel( x, y, tmpCol);
                }
                else{
                    outputQImage->setPixel( y, x, tmpCol);
                }

                D += 2 * deltaY - 2 * deltaX;
            }
            else{

                return;
            }
        }
        else{

            if( ( x < inputImage->width()) && ( x >= 0) && ( y < inputImage->height()) && ( y >= 0) ){

                //std::cout << "p: " << x << " " << y <<std::endl;

                tmpCol = *lineIterator;

                lineIterator++;

                if( ( GetOctantFromAngle( glitchParameters->sortingAngle) == 0) || ( GetOctantFromAngle( glitchParameters->sortingAngle) == 7) ){

                    outputQImage->setPixel( x, y, tmpCol);
                }
                else{
                    outputQImage->setPixel( y, x, tmpCol);
                }

                D += 2 * deltaY;
            }
            else{

                return;
            }
        }
    }
}

int GlilcthLabCore::GetOctantFromAngle(float angle){

    if(( angle >= 0.0) && (angle < 45.0)){

        return 0;
    }
    else if(( angle >= 45.0) && (angle < 90.0)){

        return 1;
    }
    else if(( angle >= 90.0) && (angle < 135.0)){

        return 2;
    }
    else if(( angle >= 135.0) && (angle < 180.0)){

        return 3;
    }
    else if(( angle >= -45.0) && (angle < 0.0)){

        return 7;
    }
    else if(( angle >= -90.0) && (angle < -45.0)){

        return 6;
    }
    else if(( angle >= -135.0) && (angle < -90.0)){

        return 5;
    }
    else if(( angle >= -180.0) && (angle < -135.0)){

        return 4;
    }
}

//new working
void GlilcthLabCore::Bresenham( QPoint _p0, QPoint _p1, std::vector< QRgb> *line, void (*function)(int, int, std::vector< QRgb> *, QPixmap*, QImage*, int)){    

    if(_p0.x()==_p1.x() && _p0.y() == _p1.y())
        return;

    if(_p0.x() > _p1.x()){
        QPoint tmpPt = _p0;
        _p0 = _p1;
        _p1 = tmpPt;
        std::reverse(line->begin(), line->end());
    }

    int x0 = _p0.x();
    int x1 = _p1.x();
    int y0 = _p0.y();
    int y1 = _p1.y();

    int x, y;

    float lineAngle = atan( (float)(y1 - y0)/ (float)(x1 - x0)) * 180.0 / 3.14159;
    int octant = lineAngle != -1 ? GetOctantFromAngle( lineAngle):0;

    //std::cout << "lineAngle: " << lineAngle << std::endl;
    //std::cout << "octant: " << octant << std::endl;

    QPoint switchedInput0 = SwitchToOctanZeroFrom( octant, x0, y0);
    QPoint switchedInput1 = SwitchToOctanZeroFrom( octant, x1, y1);

    x0 = switchedInput0.x();
    y0 = switchedInput0.y();
    x1 = switchedInput1.x();
    y1 = switchedInput1.y();

    float dx = x1 - x0;
    float dy = y1 - y0;

    float D = 2 * dy - dx;

    int index = 0;

    QPoint switchedOutput = SwitchFromOctanZeroTo(octant, x0, y0);

    if( !isInImageRange(switchedOutput)){
        return;
    }

    function ( switchedOutput.x(), switchedOutput.y(), line, inputImage, outputQImage, index);

    index++;

    y = y0;

    for( int x = x0 + 1; x <= x1; x++){

        D += 2 * dy;

        if( D > 0){

            y++;

            D = D - 2 * dx;
        }

        switchedOutput = SwitchFromOctanZeroTo(octant, x, y);

        if( !isInImageRange(switchedOutput)){
            return;
        }

        //std::cout << switchedOutput.x() << " " << switchedOutput.y() << std::endl;

        function ( switchedOutput.x(), switchedOutput.y(), line, inputImage, outputQImage, index);

        index++;

    }
}

inline bool GlilcthLabCore::isInImageRange( QPoint p){

    return true;

    if( (p.x() < 0) || (p.x() > inputImage->width()) || (p.y() < 0) || (p.y() > inputImage->height())){
    //if( (p.x() > inputImage->width()) || (p.y() > inputImage->height())){
        return false;
    }
    else{
        return true;
    }

}

QPoint GlilcthLabCore::SwitchToOctanZeroFrom( int octant, int x, int y){

    switch ( octant){
        case 0:
        return QPoint(x ,y);
            break;

        case 1:
        return QPoint(y ,x);
            break;

        case 2:
        return QPoint(y ,-x);
            break;

        case 3:
        return QPoint(-x ,y);
            break;

        case 4:
        return QPoint(-x ,-y);
            break;

        case 5:
        return QPoint(-y ,-x);
            break;

        case 6:
        return QPoint(-y, x);
            break;

        case 7:
        return QPoint(x ,-y);
            break;
    }
}

QPoint GlilcthLabCore::SwitchFromOctanZeroTo( int octant, int x, int y){

    switch ( octant){
        case 0:
        return QPoint(x ,y);
            break;

        case 1:
        return QPoint(y ,x);
            break;

        case 2:
        return QPoint(-y ,x);
            break;

        case 3:
        return QPoint(-x ,y);
            break;

        case 4:
        return QPoint(-x ,-y);
            break;

        case 5:
        return QPoint(-y ,-x);
            break;

        case 6:
        return QPoint(y, -x);
            break;

        case 7:
        return QPoint(x ,-y);
            break;
    }
}


//GLITCH PARAMETERES
void GlilcthLabCore::LoadGlitchParameters(){

    ;
}

void GlilcthLabCore::SaveGlitchParameters(){

    ;
}

QSize GlilcthLabCore::GetInputImageSize(){

    return inputImage->size();
}

QPixmap* GlilcthLabCore::GetInputImage(){

    return inputImage;
}

void GlilcthLabCore::AddPixelToMap(std::map< int, QRgb> &map, int pixelIndex, QRgb color){

    //map.insert(std::pair< int, QRgb>( glitchParameters->sortingRule->GetColorValue(color), color));

    int colorValue = glitchParameters->sortingRule->GetColorValue(color);

    std::cout << "colorValue: " << colorValue << std::endl;

    map[colorValue] = color;
}

void GlilcthLabCore::SetSortingAngle( float angle){

    glitchParameters->sortingAngle = angle;
}

void GlilcthLabCore::SetCircularSorting( bool isCircularSorting){

    glitchParameters->isCircularSorting = isCircularSorting;
}

void GlilcthLabCore::SetCircularSortingCenter(QPointF sortingCenter)
{
    glitchParameters->sortingCenter = sortingCenter;
}

void GlilcthLabCore::SetColorToValueFunction( SortingRule sortingRule){

    glitchParameters->sortingRule->SetSortingRule( sortingRule);
}

void GlilcthLabCore::InvertSortingRule( bool invert){

    glitchParameters->invertSortingRule = invert;
}

void GlilcthLabCore::SetMask( QImage* ptrMaskImg, MaskType maskType){

    glitchParameters->mask.SetMask( ptrMaskImg, maskType);
}

void GlilcthLabCore::SetSegmentationRule(bool isSegmentationEnabled, int threshold)
{
    glitchParameters->segmentationRule = new GlitchSegmentationRule(isSegmentationEnabled, threshold);
}

//GIF FUNCTIONS

void GlilcthLabCore::DrawTestLine(){

    QPoint p0 = QPoint(250, 250);
    QPoint p1 = QPoint(499, 250);

    QRgb red = qRgb(255, 0, 0);

    std::vector< QRgb>* redLine = new std::vector< QRgb>();

    redLine->reserve(2048);

    for( int i = 0; i < 2048; i ++){

        redLine->push_back(red);
    }

    /*
    Bresenham(p0, p1, redLine, DrawPixel);

    p1 = QPoint(499, 499);
    Bresenham(p0, p1, redLine, DrawPixel);

    p1 = QPoint(499, 1);
    Bresenham(p0, p1, redLine, DrawPixel);    

    p1 = QPoint(1, 1);
    Bresenham(p0, p1, redLine, DrawPixel);

    p1 = QPoint(1, 499);
    Bresenham(p0, p1, redLine, DrawPixel);
    */

    float lineCount = 10.0;
    float lineAngleInc = 360.0 / (lineCount - 1.0);

    float lineMaxLength = sqrt( pow( inputImage->width() / 2, 2) + pow( inputImage->height() / 2, 2));

    QPoint imageCenter = QPoint( inputImage->width() / 2, inputImage->height() / 2);

    for( float lineAngle = -180.0; lineAngle < 180; lineAngle += lineAngleInc){

        std::vector< QRgb>* line = new std::vector< QRgb>();
        line->reserve( 1024);

        for( int i = 0; i < 1024; i ++){
            line->push_back(Qt::red);
        }

        p1 = imageCenter;

        p1 += QPoint( lineMaxLength * cos( lineAngle * 3.14159 / 180.0), lineMaxLength * sin( lineAngle * 3.14159 / 180.0));

        std::cout << "#Angle: " << lineAngle << std::endl;
        std::cout << "drawing (" << imageCenter.x() << ", " << imageCenter.y() << " ) , (" << p1.x() << ", " << p1.y() << " )" <<std::endl;

        Bresenham(imageCenter, p1, redLine, DrawPixel);
    }

}

QImage *GlilcthLabCore::DeformWith2DFunction(){

    float spatialPeriod = 20.0;

    QPoint center = QPoint( inputImage->size().width() / 2, inputImage->size().height() / 2);

    for( int x = 0; x < inputImage->size().width(); x++){

        for( int y = 0; y < inputImage->size().height(); y++){

            QPoint currentPoint = QPoint( x, y);

            QPoint offset = QPoint( 30, 15);

            QPoint dirToCenter = center - currentPoint;

            float distanceToCenter = dirToCenter.manhattanLength();

            float intensity = sin( 2.0 * 3.14 * distanceToCenter / spatialPeriod);

            currentPoint += offset;

            //std::cout << "### x/y : " << x << " " << y << std::endl;
            //std::cout << "x/y : " << currentPoint.x() << " " << currentPoint.y() << std::endl;

            QColor color = QColor(inputImage->toImage().pixel( currentPoint));

            outputQImage->setPixel( x, y, color.rgb());
        }
    }

     return outputQImage;
}

void GlilcthLabCore::ProcessSymmetryStart(){

    QPixmap tmpPixmap = inputImage->copy(0, 0, inputImage->width()/2, inputImage->height());

    outputQImage = new QImage(inputImage->size(), QImage::Format_RGB32);

    outputQImage->fill( QColor(228, 35, 157));

    SendProcessSymmetryDone();
}
