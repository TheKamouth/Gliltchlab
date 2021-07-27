#ifndef CONTROLPOINTGRAPHICSITEM_H
#define CONTROLPOINTGRAPHICSITEM_H

#include "ControlPoint.h"

#include <QGraphicsItem>
#include <QGraphicsSceneHoverEvent>

class CurveGraphicsItem;

class ControlPointgraphicsItem : public QGraphicsItem
{
public:
    ControlPointgraphicsItem(ControlPoint * controlPoint, CurveGraphicsItem * curveGraphicsItem);

    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) override;

    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent * event) override;

    bool IsSelected();
    void SetSelected(bool selected);

    ControlPoint * GetControlPoint();

private :
    ControlPoint * _controlPoint;

    CurveGraphicsItem * _curveGraphicsItem;

    bool _isSelected;
};

#endif // CONTROLPOINTGRAPHICSITEM_H
