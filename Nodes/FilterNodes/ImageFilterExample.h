#ifndef IMAGEFILTEREXAMPLE_H
#define IMAGEFILTEREXAMPLE_H

#include "Nodes/Node.h"
#include "Nodes/GenericNode.h"
#include "Nodes/GenScatterHierarchy.h"


#include "Nodes/ProcessorNode.h"

typedef TYPE_LIST_3(ImageInputPin, ImageOutputPin, IntInputPin) ImageFilterExamplePinsTlist ;

typedef GenScatterHierarchy< ImageFilterExamplePinsTlist, GenericNodePinHolder> ImageFilterExampleInOutPins;

//typedef NodeInOutPins<ImageInputPin, ImageOutputPin> ImageFilterExampleInOutPins;

class ImageFilterNodeExample : public ProcessorNode
{
public:
    ImageFilterNodeExample();

    // map pin index to pin name
    //static const hmm ?

    static constexpr int _pinCount = Length<ImageFilterExamplePinsTlist>();

protected:
    virtual bool BeforeProcessing() override;
    // Default implementation copies _input to _ouptut
    virtual bool ProcessInternal() override;
    virtual bool AfterProcessing() const override;

private:
    ImageFilterExampleInOutPins _imageFilterExamplePins;
};

#endif // IMAGEFILTEREXAMPLE_H
