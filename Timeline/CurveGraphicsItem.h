#ifndef CURVEGRAPHICSITEM_H
#define CURVEGRAPHICSITEM_H

#include "Curve.h"

#include <QGraphicsItem>

class CurveGraphicsItem : public QGraphicsItem
{
public:
    CurveGraphicsItem();

    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) override;

private:
    Curve * _curve;

};

#endif // CURVEGRAPHICSITEM_H
