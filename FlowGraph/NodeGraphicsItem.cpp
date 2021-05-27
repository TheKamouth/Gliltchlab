#include "NodeGraphicsItem.h"

#include <QDebug>
#include <QGraphicsSceneMouseEvent>

const QColor NodeGraphicsItem::NODE_EDGE_COLOR = QColor(156, 155, 154);
const QColor NodeGraphicsItem::NODE_SELECTED_EDGE_COLOR = QColor(156, 100, 100);
const QColor NodeGraphicsItem::NODE_BACKGROUND_COLOR = QColor(186, 184, 175);
const QColor NodeGraphicsItem::NODE_TEXT_COLOR = QColor(0,0,0);

NodeGraphicsItem::NodeGraphicsItem(Node * node) :
    _node(node)
{
    update();
    //setZValue(0.1f);
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
}

QRectF NodeGraphicsItem::boundingRect() const
{
    QPointF flowGraphScenePosition = _node->FlowGraphScenePosition();
    QRect rect = QRect(flowGraphScenePosition.x(), flowGraphScenePosition.y(), NODE_WIDTH, NODE_HEIGHT);
    return rect;
}

void NodeGraphicsItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    qDebug() << __FUNCTION__;

    QBrush painterBrush = painter->brush();

    QPointF flowGraphScenePosition = _node->FlowGraphScenePosition();
    QRect rect = QRect(flowGraphScenePosition.x(), flowGraphScenePosition.y(), NODE_WIDTH + 1, NODE_HEIGHT + 1);
    //QRect rect = QRect(0, 0, NODE_WIDTH, NODE_HEIGHT);

    painterBrush.setStyle(Qt::SolidPattern);
    if (isSelected())
    {
        painterBrush.setColor(NODE_SELECTED_EDGE_COLOR);
    }
    else
    {
        painterBrush.setColor(NODE_BACKGROUND_COLOR);
    }
    painter->setBrush(painterBrush);
    painter->drawRoundedRect( rect, 2, 2);

    painterBrush.setStyle(Qt::NoBrush);
    if (isSelected())
    {
        painterBrush.setColor(NODE_SELECTED_EDGE_COLOR);
    }
    else
    {
        painterBrush.setColor(NODE_EDGE_COLOR);
    }
    painter->setBrush(painterBrush);
    painter->drawRoundedRect( rect, 2, 2);

    painterBrush.setStyle(Qt::NoBrush);
    painterBrush.setColor(NODE_TEXT_COLOR);
    painter->setBrush(painterBrush);
    painter->drawText( rect, Qt::AlignCenter, _node->Name());
}

void NodeGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    qDebug() << __FUNCTION__ << " : " << _node->Name();
}

void NodeGraphicsItem::SetGraphicsScenePosition(QPointF graphicsScenePosititon)
{
    _node->SetFlowGraphScenePosition(graphicsScenePosititon);

    update();
}
