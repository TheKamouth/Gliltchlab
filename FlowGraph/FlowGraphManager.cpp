#include "FlowGraphManager.h"

FlowGraphManager::FlowGraphManager() :
    _flowGraph(new FlowGraph())
{
    _flowGraphSceneView.SetFlowGraph(_flowGraph);
}

void FlowGraphManager::Process()
{
    _flowGraph->Process();

    emit FlowGraphProcessed();
}

FlowGraphSceneView * FlowGraphManager::GetFlowGraphSceneView()
{
    return &_flowGraphSceneView;
}
