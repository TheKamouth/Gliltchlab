#include "ImagePeakNode.h"

ImagePeakNode::ImagePeakNode() :
    _glWidget(nullptr)
{

}

ImagePeakNode::~ImagePeakNode()
{
    if( _glWidget != nullptr)
    {
        delete _glWidget;
    }
}

QWidget *ImagePeakNode::InstantiatePeakWidget()
{
    _glWidget = new OpenGLWidget(this);

    if(Output() != nullptr)
    {
        _glWidget->SetDisplayedImage(*Output());
    }
    else if (Input() != nullptr)
    {
        _glWidget->SetDisplayedImage(*Input());
    }

    return _glWidget;
}
