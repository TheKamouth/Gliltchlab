#ifndef NODEFACTORY_H
#define NODEFACTORY_H

#include <QString>

class Node;

enum NodeType
{
    // Misc
    InputImage,

    // Images processors
    Desaturate,
    ScannerFilter,

    // Misc
    OutputImage,

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
    const QString DEFAULT_IMAGE_PATH = ":/images/Resources/Images/default.png";
    const QString DEFAULT_IMAGE_OUTPUT_PATH = QString("..");

    NodeFactory();

    Node * CreateNode(NodeType type);
    static QStringList AvailableNodeTypesNames();
    static QString NodeTypeName(NodeType nodeType);
    static QString NodeTypeName(int index);
};

#endif // NODEFACTORY_H
