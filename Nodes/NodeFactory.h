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
    ScannerFilter,

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
    const QString DEFAULT_IMAGE_INPUT_PATH = QString("D:/5_PROJETS/5_DEV/VirtualScanner/test video/frame1.png");
    const QString DEFAULT_IMAGE_OUTPUT_PATH = QString("D:/5_PROJETS/5_DEV/VirtualScanner/output");

    NodeFactory();

    Node * CreateNode(NodeType type);
    QStringList AvailableNodeTypesNames();
    QString NodeTypeName(NodeType nodeType);
    QString NodeTypeName(int index);
};

#endif // NODEFACTORY_H
