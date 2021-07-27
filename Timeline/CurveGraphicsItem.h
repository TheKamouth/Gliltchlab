#ifndef CURVEGRAPHICSITEM_H
#define CURVEGRAPHICSITEM_H

#include "Curve.h"

#include <QGraphicsItem>
#include <QList>

class TrackGraphicsItem;
class ControlPointgraphicsItem;

class CurveGraphicsItem : public QGraphicsItem
{
public:
    CurveGraphicsItem(Curve * curve, TrackGraphicsItem * trackGraphicsItem);

    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) override;

    ControlPoint * AddPoint(QPointF scenePos);
    Curve * GetCurve();

    QList<ControlPointgraphicsItem*> * GetControlPointsGraphicItem();

private:
    Curve * _curve;

    // these are "child" items*
    QList<ControlPointgraphicsItem*> _controlPointsGraphicItem;

    // = it knows its "parent"
    TrackGraphicsItem * _trackGraphicsItem;
};

#endif // CURVEGRAPHICSITEM_H
