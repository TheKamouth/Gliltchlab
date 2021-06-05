#include "NodeGraphicsItem.h"

#include "FlowGraphConstants.h"
#include "PinGraphicsItem.h"
#include "FlowGraph/DataPin.h"

#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QPointF>

NodeGraphicsItem::NodeGraphicsItem(INode * node) :
    _node(node),
    _isSelected(false)
{
    update();

    //setZValue(10.0f);

    //setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
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
    painter->setRenderHint(QPainter::Antialiasing, true);
    QBrush painterBrush = painter->brush();
    int nodeHeight = NodeHeight();
    QPointF flowGraphScenePosition = _node->FlowGraphNodePosition();

    // Whole item
    QRect wholeNodeRect = QRect(flowGraphScenePosition.x() - NODE_SELECTION_WIDTH ,
                                flowGraphScenePosition.y() - NODE_SELECTION_WIDTH,
                                NODE_WIDTH +  2.0 * NODE_SELECTION_WIDTH,
                                nodeHeight + 2.0 * NODE_SELECTION_WIDTH);

    // fill whole rect
    painterBrush.setColor(CLEAR_COLOR);
    painter->fillRect(wholeNodeRect, painterBrush);

    // selection rect
    painterBrush.setStyle(Qt::SolidPattern);
    if (_isSelected/*isSelected()*/)
    {
        painterBrush.setColor(NODE_SELECTED_EDGE_COLOR);
        painter->setBrush(painterBrush);
        painter->drawRoundedRect( wholeNodeRect, 2, 2);
    }

    // Header
    QRect headerRect = QRect( flowGraphScenePosition.x(),
                              flowGraphScenePosition.y(),
                              NODE_WIDTH,
                              NODE_HEADER_HEIGHT);

    painterBrush.setStyle(Qt::SolidPattern);
    painterBrush.setColor(NODE_HEADER_BACKGROUND_COLOR);
    painter->setBrush(painterBrush);
    painter->drawRoundedRect( headerRect, 2, 2);

    // name
    painterBrush.setStyle(Qt::NoBrush);
    painterBrush.setColor(NODE_TEXT_COLOR);
    painter->setBrush(painterBrush);
    painter->drawText( headerRect, Qt::AlignLeft | Qt::AlignTop, _node->Name());

    // performance statistics could be refreshed perdiodically and smoothed
    // last frame time spent processing
    painterBrush.setStyle(Qt::NoBrush);
    painterBrush.setColor(NODE_TEXT_COLOR);
    painter->setBrush(painterBrush);
    float lastFrameProcessingTime =_node->LastFrameProcessingTime();
    QString lastFrameProcessingTimeString = QString::number(lastFrameProcessingTime) + "ms";
    painter->drawText( headerRect, Qt::AlignRight | Qt::AlignTop, lastFrameProcessingTimeString);

    // memory used
    painterBrush.setStyle(Qt::NoBrush);
    painterBrush.setColor(NODE_TEXT_COLOR);
    painter->setBrush(painterBrush);
    float memoryConsumptionMB = _node->MemoryConsumption() / 1000000;
    QString memoryConsumptionString = QString::number(memoryConsumptionMB, 'g', 3) + "MB";
    painter->drawText( headerRect, Qt::AlignRight | Qt::AlignBottom, memoryConsumptionString);

    // Content
    QRect pinRect = QRect( flowGraphScenePosition.x(),
                           flowGraphScenePosition.y() + NODE_HEADER_HEIGHT,
                           NODE_WIDTH,
                           nodeHeight - NODE_HEADER_HEIGHT);

    // pin rect
    painterBrush.setStyle(Qt::SolidPattern);
    painterBrush.setColor(NODE_PINS_BACKGROUND_COLOR);
    painter->setBrush(painterBrush);
    painter->drawRoundedRect( pinRect, 2, 2);

    // Pin text
    painterBrush.setStyle(Qt::NoBrush);
    painterBrush.setColor(NODE_TEXT_COLOR);
    painter->setBrush(painterBrush);
}

QPointF NodeGraphicsItem::GetPinConnectionPosition(IDataPin * pin)
{
    int pinCount = _node->GetPinCount();
    for(int i = 0 ; i < pinCount ; i++)
    {
        // TODO : Use DataPinList
        /*
        IDataPin * currentPin = _node->GetDataPinAt(i);
        if (currentPin == pin)
        {
            return pin->FlowGraphNodePosition();
        }
        */
    }

    qDebug() << __FUNCTION__ << " : Cannot find pin: " << pin->Name();

    return QPointF(0.0f, 0.0f);
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
