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

    virtual void SetParameters()= 0;
    virtual void BeforeProcessing() = 0;
    virtual void AfterProcessing() = 0;
    virtual QImage * Output() = 0;

    void Process();

protected:
    virtual void ProcessInternal() = 0;
};

#endif // IIMAGEPROCESSOR_H
