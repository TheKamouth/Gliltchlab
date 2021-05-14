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

struct ViewInfo
{
    double Zoom = 0.15;
    double ZoomMin = 0.01;;
    double ZoomMax = 100.0;
    QPoint ViewCenter = QPoint(0, 0);
};

class OpenGLWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    OpenGLWidget( QWidget *parent = nullptr) : QOpenGLWidget( parent),
        _displayedPixmap(nullptr),
        _lastViewCenter(),
        rightClickPressed(false),
        displayFramingLines(true),
        showSortingCenter(false),
        sortingCenter(0.5,0.5)
    {
        _viewInfo = new ViewInfo();
    }

    ~OpenGLWidget();

    void SetDisplayedImg( QPixmap * pixmap);
    void SetDisplayedImage( QImage &image);

    void Update();

    void AddRect(std::pair< QRectF, QColor> newRect);

    void ClearRects();

    void ShowFramingLines( bool show);

    ViewInfo * GetViewInfo();
    void SetViewInfo(ViewInfo* viewInfo);

    QPixmap * getDisplayedPixmap(){return _displayedPixmap;}

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

    ViewInfo * _viewInfo;

    QPixmap * _displayedPixmap;
    QImage displayedImage;

    std::vector< std::pair< QRectF, QColor> > rects;

    QPoint _lastViewCenter;
    bool rightClickPressed;
    QPoint lastClickPosition;

    bool displayFramingLines;

    bool showSortingCenter;
    QPointF sortingCenter;
};

#endif // OPENGLWIDGET_H
