#include "FlowGraphSceneView.h"

#include "FlowGraph/NodeGraphicsItem.h"
#include "FlowGraph/PinGraphicsItem.h"
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
    for( int i = 0; i < pinCount; i++)
    {
        IDataPin * dataPin = node->GetDataPinAt(i);
        PinGraphicsItem * pinItem = new PinGraphicsItem(node, dataPin);
        pinItem->setParentItem(nodeGraphicsItem);
        _flowGraphScene.addItem(pinItem);
    }

    //_flowGraphScene.installEventFilter(this);

    //QObject::connect( nodeGraphicsItem, &NodeGraphicsItem::NodeChanged, this, &FlowGraphSceneView::OnNodeChanged);

    //node->InitializeNodeCommonWidget();

    //QObject::connect(node->CommonWidget(), &NodeCommonWidget::DeleteClicked, this, &ProcessorFlowDockWidget::DeleteNode);
    //QObject::connect(node->CommonWidget(), &NodeCommonWidget::PeakClicked, this, &ProcessorFlowDockWidget::OnPeakNodeClicked);


    //ui->vboxLayoutProcessors->insertWidget(_flowGraph->NodeCount() - 1, node->Widget());
    scale(1.0f,1.0f);
}

void FlowGraphSceneView::mousePressEvent( QMouseEvent *event)
{
    QPointF sceneClickPos = mapToScene(event->pos());
    QGraphicsItem * clickedItem = _flowGraphScene.itemAt(sceneClickPos, transform());
    QGraphicsItem * clickedItem2 = itemAt( event->pos());

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

        NodeGraphicsItem * nodeItem = dynamic_cast<NodeGraphicsItem *>(clickedItem);
        PinGraphicsItem * pinItem = dynamic_cast<PinGraphicsItem *>(clickedItem);

        // We could get top most item but we dont. Have to start with more specific items
        if (pinItem != nullptr)
        {
            qDebug() << "pin item clicked";
            _fromPinItem = pinItem;
        }
        else if(nodeItem != nullptr)
        {
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
        qDebug() << "Click at : " << sceneClickPos << " clickedItem is " << (clickedItem!=nullptr?"not":"") << "null"
                 << ". clickedItem2 "<< (clickedItem2!=nullptr?"not null":"null");

        _isLeftMouseButtonPressed = true;
        _dragItemStartPosition = QPointF(0.0f, 0.0f);

        if(clickedItem != nullptr)
        {
            _dragItemStartPosition = mapToScene(event->pos());

            // clicked on an GraphicsItem
            NodeGraphicsItem * nodeItem = dynamic_cast<NodeGraphicsItem *>(clickedItem);
            PinGraphicsItem * pinItem = dynamic_cast<PinGraphicsItem *>(clickedItem);

            // We could get top most item but we dont. Have to start with more specific items
            if (pinItem != nullptr)
            {
                qDebug() << "pin item clicked";
                _fromPinItem = pinItem;
            }
            else if(nodeItem != nullptr)
            {
                _clickedNodeItem = nodeItem;
                _leftClickedOnNodeItem = true;
                //_dragItemOffsetPosition = nodeItem->mapToItem(_dragItemStartPosition);

                if (nodeItem->isSelected() == false)
                {
                    //QGraphicsItem * selectedItem = _flowGraphScene.selectedItems()[0];
                    //NodeGraphicsItem * selectedNodeItem = dynamic_cast<NodeGraphicsItem *>(selectedItem);

                    nodeItem->SetSelected(true);

                    QPainterPath path;
                    path.addRect(nodeItem->boundingRect());

                    _flowGraphScene.SetSelectionArea( path, transform());
                    //_flowGraphScene.clearSelection();

                    //nodeItem->setSelected(false);
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
                //_leftClickedOnNodeItem = false;
                //_flowGraphScene.clearSelection();
                //nodeItem->setSelected(false);
                //QPointF newPosition = mapToScene(event->pos()) + _dragItemOffsetPosition;
                //nodeItem->Node()->SetFlowGraphScenePosition(newPosition);
            }
        }
        else
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
            // Should check if pins are from a different node
            // todo :connect pins
            qDebug() << "Connect pins";
        }
        else
        {
            _clickedNodeItem = nullptr;
            _fromPinItem = nullptr;
            _toPinItem = nullptr;
        }
    }

    //qDebug() << __FUNCTION__;
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

bool FlowGraphSceneView::eventFilter(QObject *obj, QEvent *event)
{
    //qDebug() << "event: " << event->type();

    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        qDebug("Ate key press %d", keyEvent->key());
        return true;
    }
    else
    {
        // standard event processing
        return QObject::eventFilter(obj, event);
    }
}
