#include "openglwidget.h"
#include <algorithm>

static const QColor PINK(255,20,147);

void OpenGLWidget::initializeGL(){

    // Set up the rendering context, clearColor
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void OpenGLWidget::resizeGL(int w, int h){

    //avoiding warnings
    Q_UNUSED(w)
    Q_UNUSED(h)

    update();
}

void OpenGLWidget::paintGL(){            

    //if(displayedPixmap.isNull()){
    if( displayedImage.isNull()){
        return;
    }

    painter = new QPainter( this);    

    //zoom limit:
    if( !displayedImage.isNull()){

        int onScreenImgWidth = displayedImage.width() * zoom;

        if( onScreenImgWidth < this->width() * 0.25){

            zoom = this->width() * 0.25 / displayedImage.width();
        }
    }

    float width = displayedImage.width() * zoom;
    float height = displayedImage.height() * zoom;

    float posX =  this->width() / 2.0 - width / 2.0 + viewCenter.x();
    float posY =  this->height() / 2.0 -height / 2.0 + viewCenter.y();

    QRect imgRect = QRect(posX, posY, width, height);

    painter->drawImage(imgRect, displayedImage);

    //draw image framing lines
    if( displayFramingLines){

        QLine horizontalTopLine = QLine(0, posY, this->width(), posY);
        QLine horizontalBotLine = QLine(0, posY + height, this->width(), posY + height);

        QLine verticalLeftLine = QLine(posX, 0, posX, this->height());
        QLine verticalRightLine = QLine(posX + width, 0, posX + width, this->height());

        QPen linePen = QPen(Qt::black);

        linePen.setWidth( 2);

        painter->setPen( linePen);

        painter->drawLine( horizontalTopLine);
        painter->drawLine( horizontalBotLine);
        painter->drawLine( verticalLeftLine);
        painter->drawLine( verticalRightLine);
    }

    //draw sorting center
    if (showSortingCenter)
    {
        QPen circlePen = QPen(PINK);
        circlePen .setWidth(5);
        painter->setPen(circlePen);

        painter->drawEllipse(QPointF(posX + sortingCenter.x() * width, posY + sortingCenter.y() * height), 1, 1);
    }


    painter->end();

    delete painter;
}

OpenGLWidget::~OpenGLWidget()
{
    delete displayedPixmap;
}

void OpenGLWidget::SetDisplayedImg( QPixmap *pixmap){

    displayedPixmap = pixmap;

    paintGL();
    update();
    //viewChange();
}

void OpenGLWidget::SetDisplayedImage( QImage &image){

    displayedImage = image;

    paintGL();

    update();
    //viewChange();
}

void OpenGLWidget::AddRect(std::pair< QRectF, QColor> newRect){

    rects.push_back( newRect);
}

void OpenGLWidget::ClearRects(){

    rects.clear();
}

void OpenGLWidget::wheelEvent( QWheelEvent * event){

    double angleDelta = event->angleDelta().y();

    zoom += angleDelta * 0.0005;

    if(zoom < zoomMin){
        zoom = zoomMin;
    }
    if(zoom > zoomMax){
        zoom = zoomMax;
    }

    update();

    emit viewChange();
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if( rightClickPressed){

        viewCenter = lastViewCenter + event->pos() - lastClickPosition;

        if(viewCenter.x() > this->width() * 2 ){ viewCenter.setX( this->width() * 2); }
        if(viewCenter.x() < -this->width() * 2){ viewCenter.setX( -this->width() * 2); }

        if(viewCenter.y() > this->height() * 2){ viewCenter.setY( this->height() * 2); }
        if(viewCenter.y() < -this->height() * 2){ viewCenter.setY( -this->height() * 2); }

        update();

        emit viewChange();
    }
}

void OpenGLWidget::mousePressEvent(QMouseEvent * event){

    if(event->buttons() == Qt::RightButton){

        rightClickPressed = true;
        lastClickPosition = event->pos();
        lastViewCenter = viewCenter;        
    }
    else if(event->buttons() == Qt::LeftButton){

        float width = displayedImage.width() * zoom;
        float height = displayedImage.height() * zoom;
        float posX =  this->width() / 2.0 - width / 2.0 + viewCenter.x();
        float posY =  this->height() / 2.0 -height / 2.0 + viewCenter.y();


        QPoint clickPosition = event->pos() - QPoint(posX, posY);
        if(clickPosition.x() < 0.0)
            clickPosition.setX(0.0);
        if(clickPosition.x() >  width)
            clickPosition.setX(1.0);
        if(clickPosition.y() < 0.0)
            clickPosition.setY(0.0);
        if(clickPosition.y() >height)
            clickPosition.setY(1.0);

        sortingCenter = QPointF( clickPosition.x()/width, clickPosition.y()/height);

        update();
    }
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent * event){

        rightClickPressed = false;
        lastViewCenter = viewCenter;                
}

void OpenGLWidget::ShowFramingLines( bool show){

    displayFramingLines = show;

    update();
}

ViewInfo* OpenGLWidget::getViewInfo(){

    ViewInfo* viewInfo =  new ViewInfo();
    viewInfo->zoom = zoom;
    viewInfo->zoomMin = zoomMin;
    viewInfo->zoomMax = zoomMax;
    viewInfo->viewCenter = viewCenter;

    return viewInfo;
}

void OpenGLWidget::setViewInfo(ViewInfo* viewInfo)
{
    zoom = viewInfo->zoom;
    zoomMin = viewInfo->zoomMin;
    zoomMax = viewInfo->zoomMax;
    viewCenter = viewInfo->viewCenter;

    update();
}

QPointF OpenGLWidget::getSortingCenter()
{
    return sortingCenter;
}

void OpenGLWidget::ShowSortingCenter(bool show)
{
    showSortingCenter = show;

    update();
}
