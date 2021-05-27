#include "ConnectionGraphicsItem.h"

ConnectionSlotGraphicsItem::ConnectionSlotGraphicsItem()
{

}

QRectF ConnectionSlotGraphicsItem::boundingRect() const
{
    QPointF flowGraphScenePosition = _node->FlowGraphScenePosition();
    QRect rect = QRect(flowGraphScenePosition.x(), flowGraphScenePosition.y(), NodeGraphicsItem::NODE_WIDTH, NodeGraphicsItem::NODE_HEIGHT);
    return rect;
}

void ConnectionSlotGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

}
