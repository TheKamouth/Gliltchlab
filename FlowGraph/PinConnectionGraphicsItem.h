#ifndef PINCONNECTIONGRAPHICSITEM_H
#define PINCONNECTIONGRAPHICSITEM_H

#include "PinGraphicsItem.h"

#include "Nodes/GenericNode.h"
#include "Nodes/GenericNodePin.h"

#include <QGraphicsItem>

class ConnectionGraphicsItem : public QGraphicsItem
{
public:
    ConnectionGraphicsItem();

    // why is class keyword needed ?!
    ConnectionGraphicsItem( class PinGraphicsItem * fromPin, class PinGraphicsItem * toPin);

    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) override;

    void Disconnect();
    PinGraphicsItem * FromPin();
    PinGraphicsItem * ToPin();
    PinGraphicsItem * OtherEndPin(PinGraphicsItem * pin);

private:
    PinGraphicsItem * _fromPinItem;
    PinGraphicsItem * _toPinItem;
};

#endif // PINCONNECTIONGRAPHICSITEM_H
