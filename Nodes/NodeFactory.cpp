#include "NodeFactory.h"

#include "ImageInputNode.h"
#include "ImageOutputNode.h"
#include "DesaturateFilterNode.h"

#include <QDebug>

NodeFactory::NodeFactory()
{

}

Node * NodeFactory::CreateNode(NodeType type)
{
    switch(type)
    {
        case InputImage:
            return new ImageInputNode();

        case OutputImage:
            return new ImageOutputNode();

        case Desaturate:
            return new DesaturateFilterNode();

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

        case Scanner:
            return "Scanner";

        default:
            qDebug() << "NodeFactory does not handle this NodeType";
            return "no name";
    }
}

QString NodeFactory::NodeTypeName(int index)
{
    return NodeTypeName((NodeType)index);
}
