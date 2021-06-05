#include "FlowGraph.h"

#include "Nodes/Node.h"
#include "Nodes/INode.h"

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

    UpdateFlowGraphFromDom();
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

    UpdateFlowGraphFromDom();
}

void FlowGraph::UpdateFlowGraphFromDom()
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
        INode * node = AddNode(nodeType);

        QString nodeName = attributeMap.namedItem(XML_FGF_NODE_ATTRIBUTE_NAME).nodeValue();
        QString nodePosition = attributeMap.namedItem(XML_FGF_NODE_ATTRIBUTE_POSITION).nodeValue();

        node->SetName(nodeName);

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

void FlowGraph::OnNodeInputChanged(INode * node)
{
    // Reprocess all dependent nodes
    QList<INode *> nodesToProcess = GetDependantNodeList(node);

    /*
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
    */

    qDebug() << __FUNCTION__<< "Reprocessed, node changed: " << node->Name();
}

void FlowGraph::OnNodeOutputChanged(const INode * node) const
{
    emit NodeOutputChanged(node);
}

INode * FlowGraph::AddNode(NodeType nodeType)
{
    INode * node =_nodeFactory.CreateNode(nodeType);
    //node->SetPosition(_nodes.count());

    QObject::connect(node , &INode::NodeInputChanged, this, &FlowGraph::OnNodeInputChanged);
    QObject::connect(node , &INode::NodeOutputChanged, this, &FlowGraph::OnNodeOutputChanged);

    AddNodeToDom(node);

    _nodes.append(node);

    emit NodeAdded(node);

    return node;
}

INode * FlowGraph::InsertNode(NodeType nodeType, int index)
{
    INode * node =_nodeFactory.CreateNode(nodeType);
    //node->SetPosition(_nodes.count());

    QObject::connect(node , &INode::NodeInputChanged, this, &FlowGraph::OnNodeInputChanged);
    QObject::connect(node , &INode::NodeOutputChanged, this, &FlowGraph::OnNodeOutputChanged);

    AddNodeToDom(node);

    _nodes.insert(index, node);

    emit NodeAdded(node);

    return node;
}

void FlowGraph::RemoveNode(INode * node)
{
    //_nodes.remove(node);
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

    INode * previousNode = nullptr;
    INode * node;
    for(int i = 1 ; i < _nodes.count() ; i++)
    {
        previousNode = _nodes[i-1];
        node = _nodes[i];
        //node->SetInput( previousNode->Output());

        if( node->TryProcess() == false)
        {
            qDebug() << "Failed to process node: " << node->Name();
        }
    }

    emit Processed();
}

FlowData * FlowGraph::Output()
{
    if(_nodes.count() == 0)
    {
        qDebug() << "No node to process." << Qt::endl;
        return nullptr;
    }

    int lastNodeIndex = _nodes.count() - 1;
    INode * lastNode = _nodes.at(lastNodeIndex);
    return lastNode->MainOutput();
}

QList<INode *> FlowGraph::GetDependantNodeList(INode * node)
{
    return QList<INode*>();
}

void FlowGraph::AddNodeToDom(INode * node)
{
    if( _domDocument == nullptr)
    {
        qDebug() << "DOM document is nullptr.";
        return;
    }

    QDomNode root = _domDocument->firstChild();
    QDomNodeList nodeList = _domDocument->elementsByTagName(XML_FGF_NODE_ELEMENT);

    // Node
    QDomElement nodeDomElement = _domDocument->createElement(XML_FGF_NODE_ELEMENT);
    nodeDomElement.setAttribute(XML_FGF_NODE_ATTRIBUTE_NAME, node->Name());
    nodeDomElement.setAttribute(XML_FGF_NODE_ATTRIBUTE_TYPE, node->Type());
    nodeDomElement.setAttribute(XML_FGF_NODE_ATTRIBUTE_POSITION_X, node->FlowGraphNodePosition().x());
    nodeDomElement.setAttribute(XML_FGF_NODE_ATTRIBUTE_POSITION_Y, node->FlowGraphNodePosition().y());

    // Pins
    for( int i = 0; i < NodeCount(); i++)
    {
        QList<IDataPin*> dataPinList = node->GetDataPinList();

        //QDomElement nodeDomElement = _domDocument->createElement(XML_FGF_PIN_ELEMENT);
        /*
        nodeDomElement.setAttribute(XML_FGF_NODE_ATTRIBUTE_NAME, pin->Name());
        nodeDomElement.setAttribute(XML_FGF_NODE_ATTRIBUTE_TYPE, node->Type());
        nodeDomElement.setAttribute(XML_FGF_NODE_ATTRIBUTE_POSITION_X, node->FlowGraphNodePosition().x());
        nodeDomElement.setAttribute(XML_FGF_NODE_ATTRIBUTE_POSITION_Y, node->FlowGraphNodePosition().y());
        */
    }

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

