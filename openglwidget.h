#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QPainter>
#include <QRect>
#include <QColor>
#include <QPoint>
#include <QWheelEvent>

#include <iostream>

struct ViewInfo{
    double zoom;
    double zoomMin;
    double zoomMax;

    QPoint viewCenter;
};

class OpenGLWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    OpenGLWidget( QWidget *parent) : QOpenGLWidget( parent),
        zoom(0.15),
        zoomMin(0.1),
        zoomMax(10),
        rightClickPressed(false),
        displayFramingLines(true),
        showSortingCenter(false),
        sortingCenter(0.5,0.5)
    {

        viewCenter = QPoint( 0, 0);
    }

    ~OpenGLWidget();

    void SetDisplayedImg( QPixmap * pixmap);
    void SetDisplayedImage( QImage &image);

    void Update();

    void AddRect(std::pair< QRectF, QColor> newRect);

    void ClearRects();

    void ShowFramingLines( bool show);

    ViewInfo* getViewInfo();
    void setViewInfo(ViewInfo* viewInfo);

    QPixmap * getDisplayedPixmap(){return displayedPixmap;}

    QPointF getSortingCenter();
    void ShowSortingCenter( bool show);

protected:
    void initializeGL();
    void resizeGL( int w, int h);
    void paintGL();

    virtual void wheelEvent( QWheelEvent * event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent * event);
    virtual void mouseReleaseEvent(QMouseEvent * event);

signals:
    void viewChange();    

private:
    QMatrix4x4 projectionMatrix;

    QPainter *painter;

    QPixmap * displayedPixmap;

    QImage displayedImage;

    std::vector< std::pair< QRectF, QColor> > rects;

    double zoom;
    double zoomMin;
    double zoomMax;

    QPoint lastViewCenter, viewCenter;
    bool rightClickPressed;
    QPoint lastClickPosition;

    bool displayFramingLines;

    bool showSortingCenter;
    QPointF sortingCenter;
};

#endif // OPENGLWIDGET_H
