#ifndef IIMAGEPROCESSOR_H
#define IIMAGEPROCESSOR_H

#include "Nodes/Node.h"
#include "Nodes/ImagePeakNode.h"

#include <QVector2D>

class QImage;

class ImageProcessorBase : public ImagePeakNode
{
public:
    // Node
    virtual QString Name() override { return "image processor base";}


    QString GetTempImageOutputFilePath();
};

#endif // IIMAGEPROCESSOR_H
