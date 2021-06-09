#ifndef NODEFACTORY_H
#define NODEFACTORY_H

#include <QString>

class INode;

enum NodeType
{
    ImageFilterExample,
    Desaturate,

    // Misc
    ImageInput,
    AVAILABLE_NODES_COUNT,

    // Images processors
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
    //static const QString DEFAULT_IMAGE_PATH;
    static const QString DEFAULT_IMAGE_OUTPUT_PATH;

    NodeFactory();

    static INode * CreateNode(NodeType type);
    static QStringList AvailableNodeTypesNames();
    static int AvailableNodeTypesCount();
    static QString NodeTypeName(NodeType nodeType);
    static QString NodeTypeName(int index);
};

#endif // NODEFACTORY_H
