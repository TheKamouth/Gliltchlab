#ifndef FLOWGRAPH_H
#define FLOWGRAPH_H

#include "Nodes/NodeFactory.h"

#include <QList>
#include <QImage>

#include <QDomDocument>

class Node;

class FlowGraph
{
public:
    FlowGraph();

    // fgf file handling
    void CreateNewFlowGraphFile(QString filePath);
    void SaveFlowGraphFile(QString fileName);
    void LoadFlowGraphFile(QString filePath);

    //
    int NodeCount(){ return _nodes.count();}
    Node * GetNode(int index){return _nodes[index];}

    Node * AddNode(NodeType nodeType);
    void InsertNode(Node * node, int index);
    void RemoveNode(int index);
    void Process();
    QImage * Output();

    // Only linear flowgraph are handled
    // This class might be useful in a future generalisation (11/05)
    QList<Node*> _nodes;
    QString _flowName;

    // IO
    QString _domDocumentFilePath;
    QIODevice * _flowGraphFileDevice;
    QDomDocument * _domDocument;

    void RemoveNode(Node *node);
private:
    NodeFactory _nodeFactory;

    void UpdateFgfFileWithCurrentDom();
    void AddNodeToDom(Node *node);
    void UpdateDomWithFgfFile();
    void UpdateFlowGraph();
};

#endif // FLOWGRAPH_H
