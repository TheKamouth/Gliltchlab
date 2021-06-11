#ifndef CURVEGRAPHICSITEM_H
#define CURVEGRAPHICSITEM_H

#include "Curve.h"
#include "TrackGraphicsItem.h"

#include <QGraphicsItem>

class CurveGraphicsItem : public QGraphicsItem
{
public:
    CurveGraphicsItem(Curve * curve, TrackGraphicsItem * trackGraphicsItem);

    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) override;

    void AddPoint(QPointF scenePos);

private:
    Curve * _curve;
    TrackGraphicsItem * _trackGraphicsItem;

};

#endif // CURVEGRAPHICSITEM_H
