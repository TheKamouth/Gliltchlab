#include "FlowGraphSceneView.h"

#include "FlowGraph/NodeGraphicsItem.h"
#include "FlowGraph/PinGraphicsItem.h"
#include "FlowGraph/PinConnectionGraphicsItem.h"

#include "Nodes/NodeFactory.h"
#include "Nodes/Node.h"

// TMP
#include "Nodes/FilterNodes/Desaturate/DesaturateFilterNode.h"

#include <QMouseEvent>
#include <QScrollBar>

FlowGraphSceneView::FlowGraphSceneView( QWidget *parent) :
    QGraphicsView( parent),
    _flowGraph( nullptr),
    _isMiddleMouseButtonPressed( false),
    _isLeftMouseButtonPressed( false),
    _leftClickedOnNodeItem(false),
    _dragStartPosition( QPointF( 0.0f,0.0f)),
    _dragItemStartPosition( QPointF( 0.0f,0.0f)),
    _dragItemOffsetPosition( QPointF( 0.0f,0.0f)),
    _currentScale( 1.0f),
    _clickedNodeItem(nullptr),
    _fromPinItem(nullptr),
    _toPinItem(nullptr)
{
    setScene(&_flowGraphScene);
    show();

    _contextMenu.hide();

    QObject::connect(&_flowGraphScene, &FlowGraphScene::selectionChanged, this, &FlowGraphSceneView::OnSelectionChanged);
}

FlowGraphSceneView::~FlowGraphSceneView(){}

void FlowGraphSceneView::AddNodeAction(NodeType type, QPointF nodePosition)
{
    // Create Node and add to flow
    INode * node = _flowGraph->AddNode(type);
    node->SetFlowGraphScenePosition(nodePosition);
    qDebug() << __FUNCTION__ << " at position: " << nodePosition;

    //OnProcessFlowClicked();
    //OnPeakNodeClicked(node);

    // Somehow this forces redraw
    //scale(1.0f,1.0f);
    update();
}

void FlowGraphSceneView::DeleteNodeAction(INode *node)
{
    delete node;
}

void FlowGraphSceneView::OnSelectionChanged()
{
    qDebug() << __FUNCTION__;

    // forcing redraw/update somehow
    //scale(_currentScale,_currentScale);
}

void FlowGraphSceneView::OnNodeChanged()
{
    qDebug() << __FUNCTION__;
}

void FlowGraphSceneView::SetFlowGraph( FlowGraph * flowGraph)
{
    _flowGraph = flowGraph;
    QObject::connect( _flowGraph, &FlowGraph::NodeAdded, this, &FlowGraphSceneView::OnNodeAdded);

    // TODO (or something similar, get pin creation logic out of GraphicsItem classes, this need ScatterHierarchy knowledge at compile time
    // QObject::connect( _flowGraph, &FlowGraph::NodeAdded, this, &FlowGraphSceneView::OnNodeAdded);
}

void FlowGraphSceneView::OnNodeAdded( INode * node)
{
    AddNodeWidget(node);
}

void FlowGraphSceneView::AddNodeWidget( INode * node)
{
    if(node == nullptr)
    {
        qDebug() << __FUNCTION__ << " node is nullptr";
        return;
    }

    NodeGraphicsItem * nodeGraphicsItem = new NodeGraphicsItem(node);
    _flowGraphScene.addItem(nodeGraphicsItem);

    // Create a PinGraphicsItem per pin
    int pinCount = node->GetPinCount();
    QList<IDataPin*> dataPinList = node->GetDataPinList();
    for( int i = 0; i < pinCount; i++)
    {
        IDataPin * dataPin = dataPinList[i];
        PinGraphicsItem * pinItem = new PinGraphicsItem(node, dataPin);
        pinItem->setParentItem(nodeGraphicsItem);
        _flowGraphScene.addItem(pinItem);
    }


    //QObject::connect( nodeGraphicsItem, &NodeGraphicsItem::NodeChanged, this, &FlowGraphSceneView::OnNodeChanged);

    //node->InitializeNodeCommonWidget();

    //QObject::connect(node->CommonWidget(), &NodeCommonWidget::DeleteClicked, this, &ProcessorFlowDockWidget::DeleteNode);
    //QObject::connect(node->CommonWidget(), &NodeCommonWidget::PeakClicked, this, &ProcessorFlowDockWidget::OnPeakNodeClicked);

    scale(1.0f,1.0f);
}

void FlowGraphSceneView::mousePressEvent( QMouseEvent *event)
{
    QPointF sceneClickPos = mapToScene(event->pos());
    QGraphicsItem * clickedItem = _flowGraphScene.itemAt(sceneClickPos, transform());
    NodeGraphicsItem * nodeItem = dynamic_cast<NodeGraphicsItem *>(clickedItem);
    PinGraphicsItem * pinItem = dynamic_cast<PinGraphicsItem *>(clickedItem);
    ConnectionGraphicsItem * connectionItem = dynamic_cast<ConnectionGraphicsItem *>(clickedItem);

    if (event->buttons() == Qt::MiddleButton )
    {
        _isMiddleMouseButtonPressed = true;
        _dragStartPosition = event->pos();
        _sceneRect = sceneRect();
    }
    else if (event->buttons() == Qt::RightButton)
    {
        // Build and show context menu
        _contextMenu.clear();

        // We could get top most item but we dont. Have to start with more specific items
        if (pinItem != nullptr)
        {
            qDebug() << "pin clicked";

            _fromPinItem = pinItem;

        }
        else if (connectionItem != nullptr)
        {
            qDebug() << "conection clicked";

            _fromPinItem = pinItem;
            _contextMenu.addAction( "Disconnect",
                                    [=]() -> void {
                connectionItem->Disconnect();
                _flowGraphScene.removeItem(nodeItem);
                delete connectionItem;
            });

        }
        else if(nodeItem != nullptr)
        {
            qDebug() << "node clicked";

            _contextMenu.addAction("Delete node",
                                   [=]() -> void {
                _flowGraphScene.removeItem(nodeItem);
                DeleteNodeAction(nodeItem->Node());
                delete nodeItem;
            });
        }
        else
        {
            _contextMenu.addSection("Add");

            QStringList nodeNames = NodeFactory::AvailableNodeTypesNames();
            QPointF newNodePosition = mapToScene(event->pos());

            for( int i = 0; i < NodeFactory::AvailableNodeTypesCount(); i++)
            {
                _contextMenu.addAction( nodeNames[i],
                                        [=]() -> void { AddNodeAction((NodeType)i, newNodePosition);}
                );
            }
        }

        _contextMenu.move(event->globalPos());
        _contextMenu.show();
    }
    else if( event->buttons() == Qt::LeftButton)
    {
        //qDebug() << "Click at : " << sceneClickPos << "clickedItem is " << (clickedItem!=nullptr?"not":"") << "null";

        _isLeftMouseButtonPressed = true;
        _dragItemStartPosition = QPointF(0.0f, 0.0f);

        if(clickedItem != nullptr)
        {
            _dragItemStartPosition = mapToScene(event->pos());

            // no multiselection for now
            if(clickedItem->isSelected() == false)
            {
                UnselectAll();
                _flowGraphScene.clearSelection();
            }

            // We could get top most item but we dont. Have to start with more specific items
            if (pinItem != nullptr /*&& pinItem->IsInPinRect( event->pos())*/ )
            {
                _fromPinItem = pinItem;
                //qDebug() << "pin item clicked: " << &(*_fromPinItem);
            }
            else if(nodeItem != nullptr /*&& pinItem == nullptr*/)
            {
                qDebug() << "node item clicked";
                _clickedNodeItem = nodeItem;
                _leftClickedOnNodeItem = true;
                //_dragItemOffsetPosition = nodeItem->mapToItem(_dragItemStartPosition);

                if (nodeItem->isSelected() == false)
                {
                    //QGraphicsItem * selectedItem = _flowGraphScene.selectedItems()[0];
                    //NodeGraphicsItem * selectedNodeItem = dynamic_cast<NodeGraphicsItem *>(selectedItem);

                    nodeItem->SetSelected(true);
                    emit NodeSelected(nodeItem->Node());

                    QPainterPath path;
                    path.addRect(nodeItem->boundingRect());

                    _flowGraphScene.SetSelectionArea( path, transform());
                    //_flowGraphScene.clearSelection();
                }
                else
                {
                    // unselect if there is no move
                    //nodeItem->setSelected(false);

                    // Move selected items if one selected items is clicked on
                    /*
                    QPointF sceneDragPosition = mapToScene(event->pos());
                    QPointF itemDraggedPosition = nodeItem->mapFromScene(sceneDragPosition);
                    _dragItemOffsetPosition = itemDraggedPosition;
                    qDebug() << _dragItemOffsetPosition;
                    */
                }
            }
            else
            {
                qDebug() << "another item type clicked";
                //_leftClickedOnNodeItem = false;
                //_flowGraphScene.clearSelection();
                //nodeItem->setSelected(false);
                //QPointF newPosition = mapToScene(event->pos()) + _dragItemOffsetPosition;
                //nodeItem->Node()->SetFlowGraphScenePosition(newPosition);
            }
        }
        else
        {
            //qDebug() << "no item clicked";

            // Unselect All
            /*
            QList<QGraphicsItem *> allItems = _flowGraphScene.items();
            for (int i = 0 ; i < allItems.count(); i++)
            {
                QGraphicsItem * item = allItems.at(i);
                NodeGraphicsItem * nodeGraphicsItem = dynamic_cast<NodeGraphicsItem *>(item);
                if (nodeGraphicsItem != nullptr)
                {
                    nodeGraphicsItem->SetSelected(false);
                }
            }
            */

            UnselectAll();
            _flowGraphScene.clearSelection();

            _leftClickedOnNodeItem = false;
        }
    }
}

void FlowGraphSceneView::mouseMoveEvent(QMouseEvent * event)
{
    if(_isMiddleMouseButtonPressed)
    {
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - (event->x() - _dragStartPosition.x()));
        verticalScrollBar()->setValue(verticalScrollBar()->value() - (event->y() - _dragStartPosition.y()));

        _dragStartPosition = QPointF(event->x(), event->y());

    }
    else if(_isLeftMouseButtonPressed && _clickedNodeItem /*&& _flowGraphScene.selectedItems().isEmpty() == false*/)
    {
        NodeGraphicsItem * nodeGraphicsItem = dynamic_cast<NodeGraphicsItem *>(_clickedNodeItem);
        QPointF sceneDragPosition = mapToScene(event->pos());
        //QPointF itemDraggedPosition = nodeGraphicsItem->mapFromScene(sceneDragPosition);
        //QPointF itemDragLocalPosition = nodeGraphicsItem->mapFro(sceneDragPosition);
        QPointF newScenePosition = QPointF( event->x() - _dragStartPosition.x(), event->y() - _dragStartPosition.y());

        //qDebug() << _dragItemOffsetPosition;

        //sceneDragPosition += QPointF( _dragItemOffsetPosition.x(), -_dragItemOffsetPosition.y() );

        nodeGraphicsItem->SetGraphicsScenePosition( sceneDragPosition /*- _dragItemOffsetPosition*/);

        nodeGraphicsItem->update();
        _flowGraphScene.update();
    }
}

void FlowGraphSceneView::mouseReleaseEvent(QMouseEvent *event)
{
    QPointF sceneClickPos = mapToScene(event->pos());

    QGraphicsItem * clickedItem = _flowGraphScene.itemAt(sceneClickPos, transform());
    NodeGraphicsItem * nodeItem = dynamic_cast<NodeGraphicsItem *>(clickedItem);
    PinGraphicsItem * pinItem = dynamic_cast<PinGraphicsItem *>(clickedItem);

    if(event->button() == Qt::MiddleButton)
    {
        _isMiddleMouseButtonPressed = false;
        _dragStartPosition = QPointF( 0.0f, 0.0f);
    }
    else if(event->button() == Qt::LeftButton)
    {
        _isLeftMouseButtonPressed = false;

        if(event->pos() == _dragStartPosition)
        {
            // unselect if there is no move
            if(_selectedItem != nullptr)
            {
                _selectedItem->setSelected(false);
                _dragItemOffsetPosition = QPointF(0.0f, 0.0f);
            }
        }

        _dragStartPosition = QPointF( 0.0f, 0.0f);
        _dragItemOffsetPosition = QPointF(0.0f, 0.0f);

        if(pinItem != nullptr && _fromPinItem != nullptr)
        {
            _toPinItem = pinItem;

            ConnectPins( _fromPinItem, _toPinItem);

            _fromPinItem = nullptr;
            _toPinItem = nullptr;
        }
        else
        {
            _clickedNodeItem = nullptr;

            _fromPinItem = nullptr;
            _toPinItem = nullptr;
        }
    }

}

void FlowGraphSceneView::wheelEvent(QWheelEvent * event)
{
    //qDebug() << __FUNCTION__ << _currentScale;

    QPoint angleDelta = event->angleDelta();

    if(angleDelta.y() > 0)
    {
        _currentScale = 1.1f;
    }
    else
    {
        _currentScale = 0.9f;
    }

    scale(_currentScale, _currentScale);
}

void FlowGraphSceneView::ConnectPins(PinGraphicsItem * pinA, PinGraphicsItem * pinB)
{
    // This shoudl be a function, where ?
    // actually connect nodes : meaning ?
    if (pinA == pinB)
    {
        qDebug() << "Cannot connect pin with self...";
    }
    else if(pinA->Pin()->Type() != pinB->Pin()->Type())
    {
        // There could be some cast (or simple converter node)
        qDebug() << "Cannot connect pins of different types.";
    }
    else if (pinA->Pin()->IsInput() == pinB->Pin()->IsInput())
    {
        QString directionType = pinA->Pin()->IsInput() ? "input" : "output" ;
        qDebug() << "Cannot connect " << directionType << " pin with " << directionType << " pin.";
    }
    else
    {
        if(pinA->Pin()->IsInput())
        {
            PinGraphicsItem & tmp = *pinA;
            pinA = pinB;
            pinB = &tmp;

            // is input already connected ?
            // if so, reset input side pin value to default
        }

        ConnectionGraphicsItem * connectionItem = new ConnectionGraphicsItem(pinA, pinB);
        _flowGraphScene.addItem(connectionItem);

        pinA->SetConnection(connectionItem);
        pinB->SetConnection(connectionItem);

        qDebug() << "Pins connected";
    }
}

void FlowGraphSceneView::UnselectAll()
{
    // Unselect All
    QList<QGraphicsItem *> allItems = _flowGraphScene.items();
    for (int i = 0 ; i < allItems.count(); i++)
    {
        QGraphicsItem * item = allItems.at(i);
        NodeGraphicsItem * nodeGraphicsItem = dynamic_cast<NodeGraphicsItem *>(item);
        if (nodeGraphicsItem != nullptr)
        {
            nodeGraphicsItem->SetSelected(false);
        }
    }
}
