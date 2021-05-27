#include "NodeFactory.h"

#include "Nodes/Node.h"
#include "Nodes/ImageInputNode.h"
#include "Nodes/ImageOutputNode.h"
#include "Nodes/FilterNodes/Desaturate/DesaturateFilterNode.h"
#include "Nodes/FilterNodes/ScannerFilterNode.h"
#include "Nodes/FilterNodes/PixelSort/PixelSortFilterNode.h"

#include <QDebug>
#include <QCoreApplication>

const QString NodeFactory::DEFAULT_IMAGE_PATH = ":/images/Resources/Images/default.png";
const QString NodeFactory::DEFAULT_IMAGE_OUTPUT_PATH = "..";

NodeFactory::NodeFactory()
{

}

class Node * NodeFactory::CreateNode(NodeType type)
{
    class Node * node = nullptr;

    switch(type)
    {
        case ImageInput:
        {
            node = new ImageInputNode();
            ImageInputNode * inputImageNode = dynamic_cast<ImageInputNode *>(node);
            QString defaultInputImagePath = DEFAULT_IMAGE_PATH;
            inputImageNode->SetInputFilePath(defaultInputImagePath);
            break;
        }

        case ImageOutput:
        {
            node = new ImageOutputNode();
            ImageOutputNode * outputImageNode = dynamic_cast<ImageOutputNode *>(node);
            outputImageNode->SetOutputFilePath(QCoreApplication::applicationDirPath());
            break;
        }

        case Desaturate:
            node = new DesaturateFilterNode();
            break;

        case ScannerFilter:
            node = new ScannerFilterNode();
            break;

        case PixelSort:
            node = new PixelSortFilterNode();
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

    for(int i = 0 ; i < AvailableNodeTypesCount() ; i++)
    {
        availableNodeTypesNames.append(NodeTypeName(i));
    }

    return availableNodeTypesNames;
}

int NodeFactory::AvailableNodeTypesCount()
{
    return AllRed;
    // return COUNT
}

QString NodeFactory::NodeTypeName(NodeType nodeType)
{
    switch(nodeType)
    {
        case ImageInput:
            return "Image Input";

        case ImageOutput:
            return "Output Image";

        case Desaturate:
            return "Desaturate";

        case Contrast:
            return "Contrast";

        case AllRed:
            return "All Red";

        case ScannerFilter:
            return "Scanner";

        case PixelSort:
            return "PixelSort";

        default:
            qDebug() << "unnamed";
            return "no name";
    }
}

QString NodeFactory::NodeTypeName(int index)
{
    return NodeTypeName((NodeType)index);
}
