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
    switch(type)
    {
        case InputImage:
        {
            ImageInputNode * imageInputNode = new ImageInputNode();
            imageInputNode->SetInputFilePath(DEFAULT_IMAGE_INPUT_PATH);
            return imageInputNode;
        }

        case OutputImage:
        {
            ImageOutputNode * imageOutputNode = new ImageOutputNode();
            imageOutputNode->SetOutputFilePath(DEFAULT_IMAGE_OUTPUT_PATH);
            return imageOutputNode;
        }

        case Desaturate:
            return new DesaturateFilterNode();

        case ScannerFilter:
            return new ScannerFilterNode();

        default:
            qDebug() << "NodeFactory does not handle this NodeType";
            return nullptr;
    }
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
