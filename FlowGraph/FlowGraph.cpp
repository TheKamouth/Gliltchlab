#include "FlowGraph.h"

#include "Nodes/Node.h"

#include "FlowGraphXmlConstants.h"

#include <QDebug>
#include <QFile>

FlowGraph::FlowGraph() :
    _flowName("flow name"),
    _domDocumentFilePath(""),
    _flowGraphFileDevice(nullptr),
    _domDocument(nullptr)
{
}

void FlowGraph::CreateNewFlowGraphFile(QString filePath)
{
    _domDocumentFilePath = filePath;

    _domDocument = new QDomDocument(XML_FGF_ROOT);
    QDomElement root = _domDocument->createElement(XML_FGF_ROOT);
    root.setAttribute(XML_FGF_FILENAME, _flowName);
    _domDocument->appendChild(root);

    UpdateFgfFileWithCurrentDom();
}

void FlowGraph::UpdateFgfFileWithCurrentDom()
{
    if(!_flowGraphFileDevice)
    {
        //QString anotFileName = _flowName + ".fgf";
        _flowGraphFileDevice = new QFile(_domDocumentFilePath);
    }

    _flowGraphFileDevice->open(QIODevice::ReadOnly | QIODevice::Text | QIODevice::ReadWrite);

    QTextStream stream(_flowGraphFileDevice);

    stream << _domDocument->toString(4);

    _flowGraphFileDevice->close();

    // UpdateUI
}

void FlowGraph::UpdateDomWithFgfFile()
{
    _domDocument = new QDomDocument(XML_FGF_ROOT);

    if(!_flowGraphFileDevice)
    {
        _flowGraphFileDevice = new QFile(_domDocumentFilePath);
    }

    if(!_flowGraphFileDevice->open(QIODevice::ReadOnly | QIODevice::Text | QIODevice::ReadWrite))
    {
        qDebug() << "failed to open anot file.";
    }

    QString errorMsg;
    if(!_domDocument->setContent(_flowGraphFileDevice, &errorMsg))
    {
        qDebug() << "failed to parse anot file: " << errorMsg;
    }

    _flowGraphFileDevice->close();

    // UpdateUI
    UpdateFlowGraph();
}

void FlowGraph::UpdateFlowGraph()
{
    // Clear previous nodes
    for(int i = _nodes.count() - 1 ; i >= 0 ; i++)
    {
        RemoveNode(i);
    }

    if( _domDocument == nullptr)
    {
        qDebug() << "DOM document is nullptr.";
        return;
    }

    QDomNode root = _domDocument->firstChild();
    QDomNodeList nodeList = _domDocument->elementsByTagName(XML_FGF_NODE_ELEMENT);

    for(int i = 0 ; i < nodeList.count(); i++)
    {
        QDomNode domNode = nodeList.at(i);
        QStringList attributes;
        QDomNamedNodeMap attributeMap = domNode.attributes();

        QString nodeTypeString = attributeMap.namedItem(XML_FGF_NODE_ATTRIBUTE_TYPE).nodeValue();
        NodeType nodeType = (NodeType)nodeTypeString.toInt();
        Node * node = AddNode(nodeType);

        QString nodeName = attributeMap.namedItem(XML_FGF_NODE_ATTRIBUTE_NAME).nodeValue();
        QString nodePosition = attributeMap.namedItem(XML_FGF_NODE_ATTRIBUTE_POSITION).nodeValue();

        node->SetPosition(nodePosition.toInt());
        //node->SetName(nodeName);
    }
}

void FlowGraph::SaveFlowGraphFile(QString fileName)
{
    qDebug() << __FUNCTION__ << " saved to : " << fileName;

    UpdateFgfFileWithCurrentDom();
}

void FlowGraph::LoadFlowGraphFile(QString filePath)
{
    qDebug() << __FUNCTION__ << " load from : " << filePath;

    _domDocumentFilePath = filePath;

    UpdateDomWithFgfFile();
}

Node * FlowGraph::AddNode(NodeType nodeType)
{
    Node * node =_nodeFactory.CreateNode(nodeType);

    AddNodeToDom(node);

    _nodes.append(node);

    return node;
}

void FlowGraph::InsertNode(Node * node, int index)
{
    _nodes.insert(index, node);
}

void FlowGraph::RemoveNode(Node * node)
{
    _nodes.removeAll(node);
}

void FlowGraph::RemoveNode(int index)
{
    _nodes.removeAt(index);
}

void FlowGraph::Process()
{
    if(_nodes.count() == 0)
    {
        qDebug() << "No node to process." << Qt::endl;
        return;
    }

    if(_nodes.count() == 1)
    {
        return;
    }

    Node * previousNode = nullptr;
    Node * lastNode = nullptr;
    Node * node;
    for(int i = 1 ; i < _nodes.count() ; i++)
    {
        previousNode = _nodes[i-1];
        node = _nodes[i];
        node->SetInput( previousNode->Output());

        if( node->TryProcess() == false)
        {
            qDebug() << "Failed to process node: " << node->Name();
        }


        lastNode = node;
    }

    lastNode = node;
}

QImage *FlowGraph::Output()
{
    if(_nodes.count() == 0)
    {
        qDebug() << "No node to process." << Qt::endl;
        return nullptr;
    }

    int lastNodeIndex = _nodes.count() - 1;
    Node * lastNode = _nodes.at(lastNodeIndex);
    return lastNode->Output();
}

void FlowGraph::AddNodeToDom(Node * node)
{
    if( _domDocument == nullptr)
    {
        qDebug() << "DOM document is nullptr.";
        return;
    }

    QDomNode root = _domDocument->firstChild();
    QDomNodeList nodeList = _domDocument->elementsByTagName(XML_FGF_NODE_ELEMENT);

    QDomElement nodeDomElement = _domDocument->createElement(XML_FGF_NODE_ELEMENT);
    nodeDomElement.setAttribute(XML_FGF_NODE_ATTRIBUTE_NAME, node->Name());
    nodeDomElement.setAttribute(XML_FGF_NODE_ATTRIBUTE_TYPE, node->Type());
    nodeDomElement.setAttribute(XML_FGF_NODE_ATTRIBUTE_POSITION, node->FlowGraphNodePosition());

    root.appendChild(nodeDomElement);

    UpdateFgfFileWithCurrentDom();
}

