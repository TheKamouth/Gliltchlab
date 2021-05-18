#ifndef IMAGEPEAK_H
#define IMAGEPEAK_H

#include "openglwidget.h"

#include  "Node.h"

class ImagePeakNode : public Node
{
public:
    ImagePeakNode();
    virtual ~ImagePeakNode();

    virtual QWidget * InstantiatePeakWidget() override;

protected:
    virtual void UpdatePeakWidget() override;
    virtual void ReleasePeakWidget() override;

protected:
    OpenGLWidget * _glWidget;
};

#endif // IMAGEPEAK_H
