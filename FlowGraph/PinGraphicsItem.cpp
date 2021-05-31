#include "PinGraphicsItem.h"

#include "FlowGraphConstants.h"

#include <QTextOption>
#include <QPainter>

PinGraphicsItem::PinGraphicsItem()
{

}

PinGraphicsItem::PinGraphicsItem(INode * node, IDataPin * pin) : _node(node), _pin(pin)
{

}

QRectF PinGraphicsItem::boundingRect() const
{
    // Should be removed when GenericNode::GetPinAtIndex works
    if(_pin == nullptr)
    {
        qDebug() << __FUNCTION__ << " : _pin is null";
        return QRectF();
    }

    QPointF nodePosition = _node->FlowGraphNodePosition();
    int pinVerticalIndex = _node->GetPinVerticalIndex(_pin);

    float xPosition = NODE_PIN_MARGIN;
    float yPosition = NODE_HEIGHT_PER_INPUT * pinVerticalIndex + NODE_PIN_MARGIN;
    float width = NODE_PIN_TEXT_WIDTH;
    float height = NODE_HEIGHT_PER_INPUT;
    QTextOption option;

    // _pin should not be nullptr, issue with GenericNode::GetPinDataAt
    if(_pin == nullptr || _pin->IsInput())
    {
        xPosition = NODE_PIN_MARGIN;
        option.setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    }
    else
    {
        xPosition = NODE_PIN_MARGIN;
        option.setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    }


    QRect pinRect = QRect(_node->FlowGraphNodePosition().x() + xPosition,
                                _node->FlowGraphNodePosition().y() + NODE_HEADER_HEIGHT,
                                PIN_RADIUS,
                                PIN_RADIUS);

    /*
    QRect pinTextRect = QRect(_node->FlowGraphNodePosition().x() - NODE_SELECTION_WIDTH ,
                                _node->FlowGraphNodePosition().y() - NODE_SELECTION_WIDTH,
                                NODE_WIDTH +  2.0 * NODE_SELECTION_WIDTH,
                                height + 2.0 * NODE_SELECTION_WIDTH);
    */
    return pinRect;
}

void PinGraphicsItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    // Should be removed when GenericNode::GetPinAtIndex works
    if(_pin == nullptr)
    {
        qDebug() << __FUNCTION__ << " : _pin is null";
        return;
    }

    QPointF nodePosition = _node->FlowGraphNodePosition();
    int pinVerticalIndex = _node->GetPinVerticalIndex(_pin);

    float xPosition = NODE_PIN_MARGIN;
    float yPosition = NODE_HEADER_HEIGHT + NODE_HEIGHT_PER_INPUT * pinVerticalIndex + NODE_PIN_MARGIN;
    float width = NODE_WIDTH / 2.0 - NODE_PIN_MARGIN;
    float height = NODE_HEIGHT_PER_INPUT;
    QTextOption textOption;

    if(_pin->IsInput())
    {
        xPosition = NODE_PIN_MARGIN;
        textOption.setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    }
    else
    {
        // middle of width
        xPosition = width + NODE_PIN_MARGIN;
        textOption.setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    }


    QRect pinRect = QRect(_node->FlowGraphNodePosition().x() + xPosition,
                                _node->FlowGraphNodePosition().y(),
                                PIN_RADIUS,
                                PIN_RADIUS);

    QRectF pinNameRect = QRectF(_node->FlowGraphNodePosition().x() + xPosition,
                                _node->FlowGraphNodePosition().y() + yPosition,
                                width,
                                height);

    painter->drawText( pinNameRect, _pin->Name(), textOption);
}
