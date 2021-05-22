#include "openglwidget.h"
#include <algorithm>

static const QColor PINK(255,0,255);

OpenGLWidget::OpenGLWidget( QWidget *parent) : QOpenGLWidget( parent),
    _displayedImage(nullptr),
    _lastViewCenter(),
    rightClickPressed(false),
    displayFramingLines(true),
    showSortingCenter(false),
    sortingCenter(0.5,0.5)
{
    _viewInfo = new ViewInfo();
}

void OpenGLWidget::initializeGL()
{
    // Set up the rendering context, clearColor
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void OpenGLWidget::resizeGL(int w, int h)
{

    //avoiding warnings
    Q_UNUSED(w)
    Q_UNUSED(h)

    update();
}

void OpenGLWidget::paintGL()
{

    //if(displayedPixmap.isNull()){
    if( _displayedImage == nullptr || _displayedImage->isNull())
    {
        return;
    }

    painter = new QPainter( this);

    //zoom limit:
    int onScreenImgWidth = _displayedImage->width() * _viewInfo->Zoom;
    if( onScreenImgWidth < this->width() * 0.25){

        _viewInfo->Zoom = this->width() * 0.25 / _displayedImage->width();
    }

    float width = _displayedImage->width() * _viewInfo->Zoom;
    float height = _displayedImage->height() * _viewInfo->Zoom;

    float posX =  this->width() / 2.0 - width / 2.0 + _viewInfo->ViewCenter.x();
    float posY =  this->height() / 2.0 -height / 2.0 + _viewInfo->ViewCenter.y();

    QRect imgRect = QRect(posX, posY, width, height);

    painter->drawImage(imgRect, *_displayedImage);

    //draw image framing lines
    if( displayFramingLines)
    {

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
}

void OpenGLWidget::SetDisplayedImage( QImage * image){

    _displayedImage = image;

    paintGL();

    Update();
    //viewChange();
    emit viewChange();
}

void OpenGLWidget::Update()
{
    update();

    emit viewChange();
}

void OpenGLWidget::AddRect(std::pair< QRectF, QColor> newRect){

    rects.push_back( newRect);
}

void OpenGLWidget::ClearRects(){

    rects.clear();
}

void OpenGLWidget::wheelEvent( QWheelEvent * event){

    double angleDelta = event->angleDelta().y();

    _viewInfo->Zoom += angleDelta * 0.0005 ;//* _viewInfo->Zoom;

    if(_viewInfo->Zoom < _viewInfo->ZoomMin)
    {
        _viewInfo->Zoom = _viewInfo->ZoomMin;
    }
    else if(_viewInfo->Zoom > _viewInfo->ZoomMax)
    {
        _viewInfo->Zoom = _viewInfo->ZoomMax;
    }

    paintGL();
    update();

    emit viewChange();
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if( rightClickPressed){

        _viewInfo->ViewCenter = _lastViewCenter + event->pos() - lastClickPosition;

        if(_viewInfo->ViewCenter.x() > this->width() * 2 ){ _viewInfo->ViewCenter.setX( this->width() * 2); }
        if(_viewInfo->ViewCenter.x() < -this->width() * 2){ _viewInfo->ViewCenter.setX( -this->width() * 2); }

        if(_viewInfo->ViewCenter.y() > this->height() * 2){ _viewInfo->ViewCenter.setY( this->height() * 2); }
        if(_viewInfo->ViewCenter.y() < -this->height() * 2){ _viewInfo->ViewCenter.setY( -this->height() * 2); }

        update();

        emit viewChange();
    }
}

void OpenGLWidget::mousePressEvent(QMouseEvent * event)
{

    if(event->buttons() == Qt::RightButton){

        rightClickPressed = true;
        lastClickPosition = event->pos();
        _lastViewCenter = _viewInfo->ViewCenter;
    }
    else if(event->buttons() == Qt::LeftButton){

        float width = _displayedImage->width() * _viewInfo->Zoom;
        float height = _displayedImage->height() * _viewInfo->Zoom;
        float posX =  this->width() / 2.0 - width / 2.0 + _viewInfo->ViewCenter.x();
        float posY =  this->height() / 2.0 -height / 2.0 + _viewInfo->ViewCenter.y();


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

void OpenGLWidget::mouseReleaseEvent(QMouseEvent * event)
{
    rightClickPressed = false;
    _lastViewCenter = _viewInfo->ViewCenter;
}

void OpenGLWidget::ShowFramingLines( bool show){

    displayFramingLines = show;

    update();
}

ViewInfo * OpenGLWidget::GetViewInfo()
{
    return _viewInfo;
}

void OpenGLWidget::SetViewInfo(ViewInfo * viewInfo)
{
    if(_viewInfo)
    {
        _viewInfo->Zoom = viewInfo->Zoom;
        _viewInfo->ZoomMin = viewInfo->ZoomMin;
        _viewInfo->ZoomMax = viewInfo->ZoomMax;
        _viewInfo->ViewCenter = viewInfo->ViewCenter;

        paintGL();
        update();
    }
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
