#include "FlowGraphSceneView.h"

#include "FlowGraph/NodeGraphicsItem.h"
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
    _dragItemOffsetPosition(QPointF( 0.0f,0.0f)),
    _currentScale( 1.0f)
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
    class Node * node = _flowGraph->AddNode(type);
    node->SetFlowGraphScenePosition(nodePosition);
    qDebug() << __FUNCTION__ << " at position: " << nodePosition;

    //OnProcessFlowClicked();
    //OnPeakNodeClicked(node);

    // Somehow this forces redraw
    scale(1.0f,1.0f);
    update();
}

void FlowGraphSceneView::OnSelectionChanged()
{
    qDebug() << __FUNCTION__;
}

void FlowGraphSceneView::SetFlowGraph( FlowGraph * flowGraph)
{
    _flowGraph = flowGraph;
    QObject::connect( _flowGraph, &FlowGraph::NodeAdded, this, &FlowGraphSceneView::OnNodeAdded);
}

void FlowGraphSceneView::OnNodeAdded( class Node * node)
{
    AddNodeWidget(node);
}

void FlowGraphSceneView::AddNodeWidget( class Node * node)
{
    if(node == nullptr)
    {
        qDebug() << __FUNCTION__ << " node is nullptr";
        return;
    }

    NodeGraphicsItem * nodeGraphicsItem = new NodeGraphicsItem(node);
    _flowGraphScene.addItem(nodeGraphicsItem);

    //node->InitializeNodeCommonWidget();

    //QObject::connect(node->CommonWidget(), &NodeCommonWidget::DeleteClicked, this, &ProcessorFlowDockWidget::DeleteNode);
    //QObject::connect(node->CommonWidget(), &NodeCommonWidget::PeakClicked, this, &ProcessorFlowDockWidget::OnPeakNodeClicked);

    //ui->vboxLayoutProcessors->insertWidget(_flowGraph->NodeCount() - 1, node->Widget());
}

void FlowGraphSceneView::mousePressEvent( QMouseEvent *event)
{
    QGraphicsItem * clickedItem = _flowGraphScene.itemAt( _dragStartPosition, transform());
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

        if( clickedItem)
        {
            _contextMenu.addSection("Add");

            QStringList nodeNames = NodeFactory::AvailableNodeTypesNames();
            QPointF newNodePosition = mapToScene(event->pos());

            for( int i = 0; i < NodeFactory::AvailableNodeTypesCount(); i++)
            {
                _contextMenu.addAction(
                            nodeNames[i],
                            [=]() -> void { AddNodeAction((NodeType)i, newNodePosition);}
                );
            }
        }

        _contextMenu.move(event->globalPos());
        _contextMenu.show();
    }
    else if( event->buttons() == Qt::LeftButton)
    {
        _isLeftMouseButtonPressed = true;
        _dragStartPosition = mapToScene(event->pos());


        NodeGraphicsItem * nodeItem = dynamic_cast<NodeGraphicsItem *>(clickedItem);
        if(nodeItem != nullptr)
        {
            _leftClickedOnNodeItem = true;

            if (_flowGraphScene.selectedItems().count() != 0)
            {
                QGraphicsItem * selectedItem = _flowGraphScene.selectedItems()[0];
                NodeGraphicsItem * selectedNodeItem = dynamic_cast<NodeGraphicsItem *>(selectedItem);

                _flowGraphScene.clearSelection();

                nodeItem->setSelected(false);
            }
            else
            {
                nodeItem->setSelected(true);

                QPointF sceneDragPosition = mapToScene(event->pos());
                QPointF itemDraggedPosition = nodeItem->mapFromScene(sceneDragPosition);
                _dragItemOffsetPosition = itemDraggedPosition;
                qDebug() << _dragItemOffsetPosition;
            }

        }
        else
        {
            _leftClickedOnNodeItem = false;
            _flowGraphScene.clearSelection();
            //nodeItem->setSelected(false);
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
    else if(_isLeftMouseButtonPressed && _leftClickedOnNodeItem /*&& _flowGraphScene.selectedItems().isEmpty() == false*/)
    {
        if( _flowGraphScene.selectedItems().count() == 0)
        {
            qDebug() << "No item selected";
            return;
        }

        QGraphicsItem * draggedNodeItem = _flowGraphScene.selectedItems().at(0);
        NodeGraphicsItem * nodeGraphicsItem = dynamic_cast<NodeGraphicsItem *>(draggedNodeItem);
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
    if(event->button() == Qt::MiddleButton)
    {
        _isMiddleMouseButtonPressed = false;
    }
    else if(event->button() == Qt::LeftButton)
    {
        _isLeftMouseButtonPressed = false;
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
