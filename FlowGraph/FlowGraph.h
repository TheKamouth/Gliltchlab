#ifndef FLOWGRAPH_H
#define FLOWGRAPH_H

#include "Nodes/NodeFactory.h"

#include <QList>
#include <QImage>
#include <QObject>
#include <QDomDocument>

class INode;
class FlowData;

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
    INode * GetNode(int index){return _nodes[index];}

    void OnNodeInputChanged(INode * node);
    void OnNodeOutputChanged(const INode * node) const;

    INode * AddNode(NodeType nodeType);
    INode * InsertNode(NodeType nodeType, int index);
    void RemoveNode(int index);
    void Process();
    FlowData * Output();

    // Only linear flowgraph are handled
    // This class might be useful in a future generalisation (11/05)
    QList<INode*> _nodes;
    QString _flowName;

    // IO
    QString _domDocumentFilePath;
    QIODevice * _flowGraphFileDevice;
    QDomDocument * _domDocument;

    void RemoveNode(INode *node);
    QList<INode*> GetDependantNodeList(INode * node);

signals:
    void NodeAdded(INode * node);
    void NodeOutputChanged(const INode * node) const;
    void Processed();

private:
    NodeFactory _nodeFactory;

    void UpdateFgfFileWithCurrentDom();
    void AddNodeToDom(INode *node);
    void UpdateDomWithFgfFile();
    void UpdateFlowGraphFromDom();
    void SaveDialogBeforeChangingCurrentFile();
};

#endif // FLOWGRAPH_H
