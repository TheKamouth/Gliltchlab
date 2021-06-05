#ifndef NODEGRAPHICSITEM_H
#define NODEGRAPHICSITEM_H

#include "Nodes/GenericNode.h"

#include "Nodes/NodeCommonWidget.h"

#include <QGraphicsItem>
#include <QPainter>


class NodeGraphicsItem : public QGraphicsItem
{
public:
    NodeGraphicsItem(INode * node);

    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) override;

    //QPointF GetPinConnectionPosition(IDataPin * pin);

    void SetGraphicsScenePosition(QPointF graphicsScenePosititon);
    void SetSelected(bool selected);
    INode * Node();
    float NodeHeight();

signals:
    void NodeChanged();

private :
    INode * _node;
    bool _isSelected;

};

#endif // NODEGRAPHICSITEM_H
