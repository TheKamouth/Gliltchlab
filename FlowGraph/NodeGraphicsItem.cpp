#include "NodeGraphicsItem.h"

#include "FlowGraphConstants.h"
#include "PinGraphicsItem.h"

#include <QDebug>
#include <QGraphicsSceneMouseEvent>

NodeGraphicsItem::NodeGraphicsItem(INode * node) :
    _node(node),
    _isSelected(false)
{
    update();

    setZValue(10.0f);

    //setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);

    // Create a PinGraphicsItem per pin
    int pinCount = _node->GetPinCount();
    for( int i = 0; i < pinCount; i++)
    {
        IDataPin * dataPin = _node->GetDataPinAt(i);
        PinGraphicsItem * pinItem = new PinGraphicsItem(_node, dataPin);
        pinItem->setParentItem(this);
    }
}

QRectF NodeGraphicsItem::boundingRect() const
{
    int pinCount = _node->GetPinCount();
    int nodeHeight = NODE_HEADER_HEIGHT + pinCount * NODE_HEIGHT_PER_INPUT;
    QPointF nodePosition = _node->FlowGraphNodePosition();

    QRect wholeNodeRect = QRect(nodePosition.x() - NODE_SELECTION_WIDTH ,
                                nodePosition.y() - NODE_SELECTION_WIDTH,
                                NODE_WIDTH +  2.0 * NODE_SELECTION_WIDTH,
                                nodeHeight + 2.0 * NODE_SELECTION_WIDTH);;
    return wholeNodeRect;
}

void NodeGraphicsItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    QBrush painterBrush = painter->brush();

    QPointF flowGraphScenePosition = _node->FlowGraphNodePosition();
    QRect headerRect = QRect( flowGraphScenePosition.x(),
                              flowGraphScenePosition.y(),
                              NODE_WIDTH,
                              NODE_HEADER_HEIGHT);

    int nodeHeight = NodeHeight();

    QRect pinRect = QRect( flowGraphScenePosition.x(),
                           flowGraphScenePosition.y() + NODE_HEADER_HEIGHT,
                           NODE_WIDTH,
                           nodeHeight - NODE_HEADER_HEIGHT);

    QRect wholeNodeRect = QRect(flowGraphScenePosition.x() - NODE_SELECTION_WIDTH ,
                                flowGraphScenePosition.y() - NODE_SELECTION_WIDTH,
                                NODE_WIDTH +  2.0 * NODE_SELECTION_WIDTH,
                                nodeHeight + 2.0 * NODE_SELECTION_WIDTH);

    // fill whole rect
    painterBrush.setColor(CLEAR_COLOR);
    painter->fillRect(wholeNodeRect, painterBrush);

    //qDebug() << isSelected();

    // selection rect
    painterBrush.setStyle(Qt::SolidPattern);
    if (_isSelected/*isSelected()*/)
    {
        painterBrush.setColor(NODE_SELECTED_EDGE_COLOR);
        painter->setBrush(painterBrush);
        painter->drawRoundedRect( wholeNodeRect, 2, 2);
    }

    // header rect
    painterBrush.setStyle(Qt::SolidPattern);
    painterBrush.setColor(NODE_HEADER_BACKGROUND_COLOR);
    painter->setBrush(painterBrush);
    painter->drawRoundedRect( headerRect, 2, 2);

    // Header text
    painterBrush.setStyle(Qt::NoBrush);
    painterBrush.setColor(NODE_TEXT_COLOR);
    painter->setBrush(painterBrush);
    painter->drawText( headerRect, Qt::AlignCenter, _node->Name());

    // pin rect
    painterBrush.setStyle(Qt::SolidPattern);
    painterBrush.setColor(NODE_PINS_BACKGROUND_COLOR);
    painter->setBrush(painterBrush);
    painter->drawRoundedRect( pinRect, 2, 2);

    // Pin text
    painterBrush.setStyle(Qt::NoBrush);
    painterBrush.setColor(NODE_TEXT_COLOR);
    painter->setBrush(painterBrush);

    /*
    int inputIndex = 0;
    int outputIndex = 0;

    int pinCount = _node->GetPinCount();
    for( int i = 0; i < pinCount ; i++)
    {
        IDataPin * dataPin = _node->GetDataPinAt(0);

        float xPosition = 0.0f;
        float yPosition = 0.0f;
        float width = NODE_PIN_TEXT_WIDTH;
        float height = NODE_HEIGHT_PER_INPUT;
        QTextOption option;

        float heightPerPin = (nodeHeight - NODE_HEADER_HEIGHT - NODE_PIN_MARGIN * 2 ) / _node->GetPinCount();

        if(true)
        {
            xPosition = NODE_PIN_MARGIN;
            yPosition = heightPerPin * inputIndex + NODE_PIN_MARGIN;
            option.setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            inputIndex++;
        }
        else
        {
            xPosition = NODE_PIN_MARGIN;
            yPosition = heightPerPin * outputIndex;
            option.setAlignment(Qt::AlignRight | Qt::AlignVCenter);
            outputIndex++;
        }

        QRectF pinNameRect = QRectF(pinRect.x() + xPosition,
                                    pinRect.y() + yPosition,
                                    width,
                                    height);

        painter->drawText( pinNameRect, "-", option);
    }
    */
}

void NodeGraphicsItem::SetGraphicsScenePosition(QPointF graphicsScenePosititon)
{
    _node->SetFlowGraphScenePosition(graphicsScenePosititon);

    update();
}

void NodeGraphicsItem::SetSelected(bool selected)
{
    _isSelected = selected;

    setSelected(selected);

    update();
}

INode *NodeGraphicsItem::Node()
{
    return _node;
}

float NodeGraphicsItem::NodeHeight()
{
    int verticalLineCount = std::max( _node->GetInputPinCount(), _node->GetOutputPinCount());
    return verticalLineCount * NODE_HEIGHT_PER_INPUT + NODE_HEADER_HEIGHT + 2 * NODE_PIN_MARGIN;
}
