#ifndef FLOWGRAPH_H
#define FLOWGRAPH_H

#include "Nodes/NodeFactory.h"

#include <QList>
#include <QImage>
#include <QObject>
#include <QDomDocument>

class Node;

class FlowGraph : public QObject
{
    Q_OBJECT

public:
    FlowGraph();

    // fgf file handling
    void CreateNewFlowGraphFile();
    void SaveFlowGraphFile(QString fileName);
    void LoadFlowGraphFile(QString filePath);

    //
    int NodeCount(){ return _nodes.count();}
    Node * GetNode(int index){return _nodes[index];}

    void OnNodeInputChanged(Node * node);
    void OnNodeOutputChanged(Node * node);

    Node * AddNode(NodeType nodeType);
    Node * InsertNode(NodeType nodeType, int index);
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

signals:
    void NodeAdded(Node * node);
    void NodeOutputChanged(Node * node);
    void Processed();

private:
    NodeFactory _nodeFactory;

    void UpdateFgfFileWithCurrentDom();
    void AddNodeToDom(Node *node);
    void UpdateDomWithFgfFile();
    void UpdateFlowGraph();
    void SaveDialogBeforeChangingCurrentFile();
};

#endif // FLOWGRAPH_H
