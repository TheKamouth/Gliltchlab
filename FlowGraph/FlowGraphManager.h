#ifndef FLOWGRAPHMANAGER_H
#define FLOWGRAPHMANAGER_H

#include "FlowGraph.h"
#include "FlowGraphSceneView.h"

#include <QObject>

class FlowGraphManager : public QObject
{
    Q_OBJECT;

public:
    FlowGraphManager();
    FlowGraphManager(FlowGraphManager const &) = delete;
    void operator=(FlowGraphManager const &) = delete;

    static FlowGraphManager & Instance()
    {
        static FlowGraphManager instance;
        return instance;
    }

    void Process();

    FlowGraphSceneView * GetFlowGraphSceneView();

   private:
    // Add a FlowGraphManager handling sceneView, flowgraph, .fgf
    // Here it is !
    FlowGraphSceneView _flowGraphSceneView;
    FlowGraph * _flowGraph;
};

#endif // FLOWGRAPHMANAGER_H
