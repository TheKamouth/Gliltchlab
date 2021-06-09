#ifndef FLOWGRAPHSCENEWIDGET_H
#define FLOWGRAPHSCENEWIDGET_H

#include "FlowGraph.h"
#include "FlowGraphScene.h"

#include "PinGraphicsItem.h"

#include <QMenu>
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>

class FlowGraphSceneView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit FlowGraphSceneView(QWidget *parent = nullptr);
    ~FlowGraphSceneView();

    void AddNodeAction(NodeType type, QPointF nodePosition);
    void DeleteNodeAction(INode * node);
    void OnSelectionChanged();
    void OnNodeChanged();

    void SetFlowGraph( FlowGraph * flowGraph);
    void OnNodeAdded( INode * node);
    void AddNodeWidget( INode * node);

protected:
    virtual void mousePressEvent(QMouseEvent * event) override;
    virtual void mouseMoveEvent(QMouseEvent * event) override;
    virtual void mouseReleaseEvent(QMouseEvent * event) override;
    virtual void wheelEvent(QWheelEvent * event) override;

signals:
    void NodeSelected(INode *node);

private:
    FlowGraphScene _flowGraphScene;
    FlowGraph * _flowGraph;

    bool _isMiddleMouseButtonPressed;
    bool _isLeftMouseButtonPressed;
    bool _leftClickedOnNodeItem;

    QPointF _dragStartPosition;
    QPointF _dragItemStartPosition;
    QPointF _dragItemOffsetPosition;
    QRectF _sceneRect;

    float _currentScale;

    QMenu _contextMenu;
    QGraphicsItem * _selectedItem;

    NodeGraphicsItem * _clickedNodeItem;

    //
    PinGraphicsItem * _fromPinItem;
    PinGraphicsItem * _toPinItem;

    void Repaint();
    void ConnectPins(PinGraphicsItem * pinA, PinGraphicsItem * pinB);
    void UnselectAll();
};

#endif // FLOWGRAPHSCENEWIDGET_H
