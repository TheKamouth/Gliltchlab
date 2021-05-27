#ifndef FLOWGRAPHSCENEWIDGET_H
#define FLOWGRAPHSCENEWIDGET_H

#include "FlowGraph.h"
#include "FlowGraphScene.h"

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
    void OnSelectionChanged();

    void SetFlowGraph( FlowGraph * flowGraph);
    void OnNodeAdded( Node * node);
    void AddNodeWidget( Node * node);

protected:
    virtual void mousePressEvent(QMouseEvent * event) override;
    virtual void mouseMoveEvent(QMouseEvent * event) override;
    virtual void mouseReleaseEvent(QMouseEvent * event) override;
    virtual void wheelEvent(QWheelEvent * event) override;

private:
    FlowGraphScene _flowGraphScene;
    FlowGraph * _flowGraph;

    bool _isMiddleMouseButtonPressed;
    bool _isLeftMouseButtonPressed;
    bool _leftClickedOnNodeItem;

    QPointF _dragStartPosition;
    QPointF _dragItemOffsetPosition;
    QRectF _sceneRect;

    float _currentScale;

    QMenu _contextMenu;
};

#endif // FLOWGRAPHSCENEWIDGET_H
