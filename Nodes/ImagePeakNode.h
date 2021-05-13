#ifndef IMAGEPEAK_H
#define IMAGEPEAK_H

#include "openglwidget.h"

#include  "Node.h"

class ImagePeakNode : public Node
{
public:
    ImagePeakNode();
    ~ImagePeakNode();

    virtual QWidget * InstantiatePeakWidget() override;

private:
    OpenGLWidget * _glWidget;
};

#endif // IMAGEPEAK_H
