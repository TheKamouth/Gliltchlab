#ifndef PINCONNECTIONGRAPHICSITEM_H
#define PINCONNECTIONGRAPHICSITEM_H

#include "Nodes/GenericNode.h"
#include "Nodes/GenericNodePin.h"
#include "PinGraphicsItem.h"
//#include "NodeGraphicsItem.h"

#include <QGraphicsItem>

class ConnectionGraphicsItem : public QGraphicsItem
{
public:
    ConnectionGraphicsItem();
    ConnectionGraphicsItem(PinGraphicsItem * fromPin, PinGraphicsItem * toPin);

    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) override;

private:
    PinGraphicsItem * _fromPinItem;
    PinGraphicsItem * _toPinItem;
};

#endif // PINCONNECTIONGRAPHICSITEM_H
