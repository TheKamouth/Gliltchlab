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
    void Update()
    {
        if(Output() != nullptr)
        {
            _glWidget->SetDisplayedImage(*Output());
        }
        else if (Input() != nullptr)
        {
            _glWidget->SetDisplayedImage(*Input());
        }
    }

private:
    OpenGLWidget * _glWidget;
};

#endif // IMAGEPEAK_H
