#ifndef NODEFACTORY_H
#define NODEFACTORY_H

#include <QString>

class Node;

enum NodeType
{
    // Misc
    InputImage,
    OutputImage,

    // Images processors
    Desaturate,
    Scanner,

    // Debug/Test
    AllRed,

    // TODO
    PixelSort,
    InputColor,
    Contrast,
    Curves,
    Sharpen,
    COUNT
};

class NodeFactory
{
public:
    NodeFactory();

    Node * CreateNode(NodeType type);
    QStringList AvailableNodeTypesNames();
    QString NodeTypeName(NodeType nodeType);
    QString NodeTypeName(int index);
};

#endif // NODEFACTORY_H
