#include "FlowGraph.h"

#include "Nodes/Node.h"

#include "FlowGraphXmlConstants.h"

#include <QDebug>
#include <QFile>
#include <QMessageBox>

FlowGraph::FlowGraph() :
    QObject(),
    _flowName("flow name"),
    _domDocumentFilePath(""),
    _flowGraphFileDevice(nullptr),
    _domDocument(nullptr)
{
    CreateNewFlowGraphFile();
}

void FlowGraph::CreateNewFlowGraphFile()
{
    // Clear previous nodes
    for(int i = _nodes.count() - 1 ; i >= 0 ; i--)
    {
        RemoveNode(i);
    }

    _domDocument = new QDomDocument(XML_FGF_ROOT);
    QDomElement root = _domDocument->createElement(XML_FGF_ROOT);
    root.setAttribute(XML_FGF_FILENAME, _flowName);
    _domDocument->appendChild(root);

    UpdateFgfFileWithCurrentDom();
}

void FlowGraph::UpdateFgfFileWithCurrentDom()
{
    if(_flowGraphFileDevice == nullptr)
    {
        //QString anotFileName = _flowName + ".fgf";
        _flowGraphFileDevice = new QFile(_domDocumentFilePath);
    }

    _flowGraphFileDevice->open(QIODevice::ReadOnly | QIODevice::Text | QIODevice::ReadWrite);

    QTextStream stream(_flowGraphFileDevice);

    stream << _domDocument->toString(4);

    _flowGraphFileDevice->close();

    UpdateFlowGraph();
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

    UpdateFlowGraph();
}

void FlowGraph::UpdateFlowGraph()
{
    // Clear previous nodes
    for(int i = _nodes.count() - 1 ; i >= 0 ; i--)
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

    int nodeCount = nodeList.count();
    for(int i = 0 ; i < nodeCount ; i++)
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

        emit NodeAdded(node);
    }
}

void FlowGraph::SaveFlowGraphFile(QString fileName)
{
    _domDocumentFilePath = fileName;

    // Delete existing file
    if(QFile::exists(fileName))
    {
        QFile * existingFile = new QFile(fileName);
        existingFile->remove();
    }

    qDebug() << __FUNCTION__ << " saved to : " << fileName;

    UpdateFgfFileWithCurrentDom();
}

void FlowGraph::LoadFlowGraphFile(QString filePath)
{
    qDebug() << __FUNCTION__ << " load from : " << filePath;

    _domDocumentFilePath = filePath;

    UpdateDomWithFgfFile();

    Process();
}

void FlowGraph::OnNodeInputChanged(Node * node)
{
    // Reprocess all dependent nodes
    int position = node->Position();

    if (position >= _nodes.count() || position < 0)
    {
        qDebug() << __FUNCTION__ << " invalid position." ;
        return;
    }

    Node * previousNode = nullptr;
    for(int i = std::max(1, position) ; i < _nodes.count() ; i++)
    {
        previousNode = _nodes[i-1];
        node = _nodes[i];
        node->SetInput( previousNode->Output());

        if( node->TryProcess() == false)
        {
            qDebug() << "Failed to process node: " << node->Name();
        }
    }

    qDebug() << __FUNCTION__<< "Reprocessed, node changed: " << node->Name();
}

void FlowGraph::OnNodeOutputChanged(Node * node)
{
    emit NodeOutputChanged(node);
}

Node * FlowGraph::AddNode(NodeType nodeType)
{
    Node * node =_nodeFactory.CreateNode(nodeType);
    node->SetPosition(_nodes.count());

    QObject::connect(node , &Node::NodeInputChanged, this, &FlowGraph::OnNodeInputChanged);
    QObject::connect(node , &Node::NodeOutputChanged, this, &FlowGraph::OnNodeOutputChanged);

    AddNodeToDom(node);

    _nodes.append(node);

    emit NodeAdded(node);

    return node;
}

Node * FlowGraph::InsertNode(NodeType nodeType, int index)
{
    Node * node =_nodeFactory.CreateNode(nodeType);
    node->SetPosition(_nodes.count());

    QObject::connect(node , &Node::NodeInputChanged, this, &FlowGraph::OnNodeInputChanged);
    QObject::connect(node , &Node::NodeOutputChanged, this, &FlowGraph::OnNodeOutputChanged);

    AddNodeToDom(node);

    _nodes.insert(index, node);

    emit NodeAdded(node);

    return node;
}

void FlowGraph::RemoveNode(Node * node)
{
    _nodes.removeAt(node->Position());
    delete node;
}

void FlowGraph::RemoveNode(int index)
{
    _nodes.removeAt(index);

    Process();
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
    }

    emit Processed();
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
}

void FlowGraph::SaveDialogBeforeChangingCurrentFile()
{
    if( _domDocument != nullptr)
    {
        QMessageBox msgBox;
        msgBox.setText("The document has been modified.");
        msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();

        if(ret == 0)
        {
            // Save
            SaveFlowGraphFile(_domDocumentFilePath);
        }
        if(ret == 1)
        {
            // Discard
        }
        else
        {
            // Cancel
            return;
        }
    }
}

