#ifndef PINGRAPHICSITEM_H
#define PINGRAPHICSITEM_H

#include "Nodes/GenericNode.h"
#include "Nodes/GenericNodePin.h"

#include "NodeGraphicsItem.h"
#include "PinConnectionGraphicsItem.h"

#include <QGraphicsItem>

class PinGraphicsItem : public QGraphicsItem
{
public:
    PinGraphicsItem();
    PinGraphicsItem(INode * node, IDataPin * pin);

    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) override;

    QPointF GetConnectionPosition();
    IDataPin * Pin() { return _pin;}
    void SetConnection(class ConnectionGraphicsItem * connection);
    void Disconnect();

    class ConnectionGraphicsItem * GetConnection();

private:
    IDataPin * _pin;

    PinGraphicsItem * _connectedToPin;

    class ConnectionGraphicsItem * _connection;

    // used to retrieve pin position
    INode * _node;
    NodeGraphicsItem * _nodeGraphicsItem;
};

#endif // PINGRAPHICSITEM_H
