#include "PinGraphicsItem.h"

#include "FlowGraphConstants.h"

#include <QTextOption>
#include <QPainter>

PinGraphicsItem::PinGraphicsItem()
{

}

PinGraphicsItem::PinGraphicsItem(INode * node, IDataPin * pin) :
    _pin(pin),
    _connectedToPin(nullptr),
    // _connection(nullptr),
    _node(node)
{

}

QRectF PinGraphicsItem::boundingRect() const
{
    QPointF nodePosition = _node->FlowGraphNodePosition();
    int pinVerticalIndex = _node->GetPinVerticalIndex(_pin);

    float pinHandlePositionX;
    float pinHandlePositionY = NODE_HEADER_HEIGHT + NODE_HEIGHT_PER_INPUT * pinVerticalIndex + NODE_PIN_MARGIN + NODE_HEIGHT_PER_INPUT / 2.0 - PIN_RADIUS /2.0;

    if(_pin->IsInput())
    {
        pinHandlePositionX = 0;
    }
    else
    {
        pinHandlePositionX = NODE_WIDTH - PIN_RADIUS;
    }

    QRect wholePinRect = QRect(nodePosition.x() + pinHandlePositionX,
                               nodePosition.y() + pinHandlePositionY,
                               PIN_RADIUS,
                               PIN_RADIUS);
    return wholePinRect;
}

void PinGraphicsItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    painter->setRenderHint(QPainter::Antialiasing, true);

    QPointF nodePosition = _node->FlowGraphNodePosition();
    int pinVerticalIndex = _node->GetPinVerticalIndex(_pin);

    // compute wholePinRect
    float wholeRectPositionX = 0;
    float wholeRectPositionY = NODE_HEADER_HEIGHT + NODE_HEIGHT_PER_INPUT * pinVerticalIndex + NODE_PIN_TOP_MARGIN;

    float wholePinRectWidth = NODE_WIDTH / 2.0;
    float wholePinRectHeight = NODE_HEIGHT_PER_INPUT;

    QTextOption textOption;
    float pinTextPositionX;
    float pinTextPositionY = NODE_HEADER_HEIGHT + NODE_HEIGHT_PER_INPUT * pinVerticalIndex + NODE_PIN_MARGIN;
    float pinHandlePositionX;
    float pinHandlePositionY = NODE_HEADER_HEIGHT + NODE_HEIGHT_PER_INPUT * pinVerticalIndex + NODE_PIN_MARGIN + NODE_HEIGHT_PER_INPUT / 2.0 - PIN_RADIUS /2.0;

    if(_pin->IsInput())
    {
        wholeRectPositionX = 0;
        textOption.setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        pinTextPositionX = - NODE_PIN_MARGIN - wholePinRectWidth;
        pinHandlePositionX = 0;
    }
    else
    {
        wholeRectPositionX = NODE_WIDTH / 2.0;
        textOption.setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        // middle of width
        pinTextPositionX = NODE_WIDTH + NODE_PIN_MARGIN;
        pinHandlePositionX = NODE_WIDTH - PIN_RADIUS;
    }

    QRect wholePinRect = QRect( wholeRectPositionX,
                                wholeRectPositionY,
                                wholePinRectWidth,
                                wholePinRectHeight);

    QRectF pinHandleRect = QRect( nodePosition.x() + pinHandlePositionX,
                                  nodePosition.y() + pinHandlePositionY,
                                  PIN_RADIUS,
                                  PIN_RADIUS);


    QRectF pinNameRect = QRectF( nodePosition.x() + pinTextPositionX,
                                 nodePosition.y() + pinTextPositionY,
                                 wholePinRectWidth,
                                 wholePinRectHeight);

    QBrush painterBrush = painter->brush();

    painterBrush.setStyle(Qt::SolidPattern);
    painterBrush.setColor(FlowData::TypeToColor(_pin->GetData()->GetType()));
    painter->setBrush(painterBrush);
    painter->drawRect(pinHandleRect);

    QFont bold("Verdana", 8);
    bold.setBold(true);
    painter->setFont(bold);
    painter->setPen( FlowData::TypeToColor( _pin->GetData()->GetType()));
    painter->drawText( pinNameRect, _pin->Name(), textOption);
}

QPointF PinGraphicsItem::GetConnectionPosition()
{
    QPointF nodePosition = _node->FlowGraphNodePosition();
    int pinVerticalIndex = _node->GetPinVerticalIndex(_pin);

    float pinConnectionPositionX;
    float pinConnectionPositionY = NODE_HEADER_HEIGHT + NODE_HEIGHT_PER_INPUT * pinVerticalIndex + NODE_PIN_MARGIN + NODE_HEIGHT_PER_INPUT / 2.0;

    if(_pin->IsInput())
    {
        pinConnectionPositionX = 0;
    }
    else
    {
        pinConnectionPositionX = NODE_WIDTH;
    }

    return QPointF( nodePosition.x() + pinConnectionPositionX,
                    nodePosition.y() + pinConnectionPositionY);
}

void PinGraphicsItem::SetConnection(ConnectionGraphicsItem * connectionTo)
{
    _connection = connectionTo;
    _connectedToPin = _connection->OtherEndPin(this);

    _pin->SetConnectedPin(_connectedToPin->Pin());
}

void PinGraphicsItem::Disconnect()
{
    _connection = nullptr;
}

bool PinGraphicsItem::IsInPinRect(QPoint position)
{
    QPointF nodePosition = _node->FlowGraphNodePosition();
    int pinVerticalIndex = _node->GetPinVerticalIndex(_pin);

    // compute wholePinRect
    float wholeRectPositionX = 0;
    float wholeRectPositionY = NODE_HEADER_HEIGHT + NODE_HEIGHT_PER_INPUT * pinVerticalIndex + NODE_PIN_TOP_MARGIN;

    float wholePinRectWidth = NODE_WIDTH / 2.0;
    float wholePinRectHeight = NODE_HEIGHT_PER_INPUT;

    QRect wholePinRect = QRect(nodePosition.x() + wholeRectPositionX ,
                               nodePosition.y() + wholeRectPositionY,
                               wholePinRectWidth,
                               wholePinRectHeight);

    return wholePinRect.contains(position);
}

ConnectionGraphicsItem *PinGraphicsItem::GetConnection()
{
    return _connection;
}
