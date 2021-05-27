#ifndef NODEFACTORY_H
#define NODEFACTORY_H

#include <QString>

class Node;

enum NodeType
{
    // Misc
    ImageInput,

    // Images processors
    Desaturate,
    ScannerFilter,
    PixelSort,

    // Misc
    ImageOutput,

    // Debug/Test
    AllRed,

    // TODO
    InputColor,
    Contrast,
    Curves,
    Sharpen,
    COUNT
};

class NodeFactory
{
public:
    static const QString DEFAULT_IMAGE_PATH;
    static const QString DEFAULT_IMAGE_OUTPUT_PATH;

    NodeFactory();

    static Node * CreateNode(NodeType type);
    static QStringList AvailableNodeTypesNames();
    static int AvailableNodeTypesCount();
    static QString NodeTypeName(NodeType nodeType);
    static QString NodeTypeName(int index);
};

#endif // NODEFACTORY_H
