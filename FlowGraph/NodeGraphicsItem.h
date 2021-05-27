#ifndef NODEGRAPHICSITEM_H
#define NODEGRAPHICSITEM_H

#include "Nodes/Node.h"

#include "Nodes/NodeCommonWidget.h"

#include <QGraphicsItem>
#include <QPainter>

class NodeGraphicsItem : public QGraphicsItem
{
public:
    NodeGraphicsItem(Node * node);

    static const int NODE_WIDTH = 100;
    static const int NODE_HEIGHT = 40;
    static const QColor NODE_EDGE_COLOR;
    static const QColor NODE_SELECTED_EDGE_COLOR;
    static const QColor NODE_BACKGROUND_COLOR;
    static const QColor NODE_TEXT_COLOR;

    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) override;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent * event) override;

    void SetGraphicsScenePosition(QPointF graphicsScenePosititon);

private :
    Node * _node;

};

#endif // NODEGRAPHICSITEM_H
