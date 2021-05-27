#ifndef CONNECTIONGRAPHICSITEM_H
#define CONNECTIONGRAPHICSITEM_H

#include "Nodes/Node.h"
#include "NodeGraphicsItem.h"

#include <QGraphicsItem>

class ConnectionSlotGraphicsItem : public QGraphicsItem
{
public:
    ConnectionSlotGraphicsItem();

    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) override;

private:
    Node * _node;
    NodeGraphicsItem * _nodeGraphicsItem;
};

#endif // CONNECTIONGRAPHICSITEM_H
