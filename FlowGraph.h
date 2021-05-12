#ifndef FLOWGRAPH_H
#define FLOWGRAPH_H

#include <QList>

class Node;

class FlowGraph
{
public:
    FlowGraph();

    void AddNode(Node * node);
    void InsertNode(Node * node, int index);
    void RemoveNode(int index);

    // Only linear flowgraph are handled
    // This class might be useful in a future generalisation (11/05)
    QList<Node*> _nodes;
};

#endif // FLOWGRAPH_H
