#ifndef IMAGEFILTEREXAMPLE_H
#define IMAGEFILTEREXAMPLE_H

#include "Nodes/Node.h"
#include "Nodes/GenericNode.h"
#include "Nodes/GenScatterHierarchy.h"

//typedef GenScatterHierarchy< TYPE_LIST_2(ImageInputPin, ImageOutputPin), GenericNodePinHolder> ImageExampleNodePins;
typedef NodeInOutPins<ImageInputPin, ImageOutputPin> ImageFilterExampleInOutPins;

class ImageFilterExample //: ?
{
public:
    ImageFilterExample();

protected:
    virtual bool BeforeProcessing();
    // Default implementation copies _input to _ouptut
    virtual bool ProcessInternal();
    virtual bool AfterProcessing() const;

private:
    ImageFilterExampleInOutPins _imageFilterExamplePins;

};

#endif // IMAGEFILTEREXAMPLE_H
