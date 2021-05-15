#include "NodeFactory.h"

#include "Nodes/ImageInputNode.h"
#include "Nodes/ImageOutputNode.h"
#include "Nodes/FilterNodes/DesaturateFilterNode.h"
#include "Nodes/FilterNodes/ScannerFilterNode.h"

#include <QDebug>

NodeFactory::NodeFactory()
{

}

Node * NodeFactory::CreateNode(NodeType type)
{
    Node * node = nullptr;

    switch(type)
    {
        case InputImage:
        {
            node = new ImageInputNode();
            ImageInputNode * inputImageNode = dynamic_cast<ImageInputNode *>(node);
            inputImageNode->SetInputFilePath(DEFAULT_IMAGE_INPUT_PATH);
            break;
        }

        case OutputImage:
        {
            node = new ImageOutputNode();
            ImageOutputNode * outputImageNode = dynamic_cast<ImageOutputNode *>(node);
            outputImageNode->SetOutputFilePath(DEFAULT_IMAGE_OUTPUT_PATH);
            break;
        }

        case Desaturate:
            node = new DesaturateFilterNode();
            break;

        case ScannerFilter:
            node = new ScannerFilterNode();
            break;

        default:
            qDebug() << "NodeFactory does not handle this NodeType";
            return nullptr;
            break;
    }

    node->SetType(type);
    return node;
}

QStringList NodeFactory::AvailableNodeTypesNames()
{
    QStringList availableNodeTypesNames;

    for(int i = 0 ; i < COUNT ; i++)
    {
        availableNodeTypesNames.append(NodeTypeName(i));
    }

    return availableNodeTypesNames;
}

QString NodeFactory::NodeTypeName(NodeType nodeType)
{
    switch(nodeType)
    {
        case InputImage:
            return "Image Input";

        case OutputImage:
            return "Output Image";

        case Desaturate:
            return "Desaturate";

        case Contrast:
            return "Contrast";

        case AllRed:
            return "All Red";

        case ScannerFilter:
            return "Scanner";

        default:
            qDebug() << "unnamed";
            return "no name";
    }
}

QString NodeFactory::NodeTypeName(int index)
{
    return NodeTypeName((NodeType)index);
}
