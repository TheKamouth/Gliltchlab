#ifndef IMAGEFILTEREXAMPLE_H
#define IMAGEFILTEREXAMPLE_H

#include "Nodes/Node.h"
#include "Nodes/GenericNode.h"
#include "Nodes/GenScatterHierarchy.h"

//typedef GenScatterHierarchy< TYPE_LIST_2(ImageInputPin, ImageOutputPin), GenericNodePinHolder> ImageExampleNodePins;
typedef GenericNode<ImageInputPin, ImageOutputPin> ImageFilterExamplePins;

class ImageFilterExample //: ?
{
public:
    ImageFilterExample();

private:
    ImageFilterExamplePins _imageFilterExamplePins;
};

#endif // IMAGEFILTEREXAMPLE_H
