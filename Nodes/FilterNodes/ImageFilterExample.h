#ifndef IMAGEFILTEREXAMPLE_H
#define IMAGEFILTEREXAMPLE_H

#include "Nodes/Node.h"
#include "Nodes/GenericNode.h"
#include "Nodes/GenericNodePin.h"
#include "Nodes/GenScatterHierarchy.h"
#include "Nodes/TypeList.h"

#include "Nodes/ProcessorNode.h"
/*
typedef class TYPE_LIST_3(ImageInputPin, ImageOutputPin, IntInputPin) ImageFilterExamplePinsTlist ;
class ImageFilterNodeExample : public IProcessorNode
{
public:
    ImageFilterNodeExample();

    // map pin index to pin name
    //static const hmm ?

    //typedef std::integral_constant<int, Length<TypeList<int, NullType> >::value > pinCount;
    typedef std::integral_constant<int, Length< ImageFilterExamplePinsTlist >::value > PinCount;
    typedef GenScatterHierarchy< ImageFilterExamplePinsTlist, GenericNodePinHolder> ImageFilterExampleInOutPins;

protected:
    virtual bool BeforeProcessing() override;
    // Default implementation copies _input to _ouptut
    virtual bool ProcessInternal() override;
    virtual bool AfterProcessing() const override;

private:
    ImageFilterExampleInOutPins _imageFilterExamplePins;
};
*/

/////////////////////////////////
// An other try at simplifying nodes implementation
typedef TYPE_LIST_3(ImageInputPin, ImageOutputPin, IntInputPin) ImageFilterExamplePinsTlist ;
class ImageFilterNodeExample : public GenericNode<ImageFilterExamplePinsTlist>
{
public:
    ~ImageFilterNodeExample(){};
    typedef GenericNode<ImageFilterExamplePinsTlist> SimplerImageFilterGenericNodeType;

    // INode
    virtual NodeType Type() override { return ImageFilterExample;}
    virtual QString Name() override { return "ImageFilterExample";}
    virtual FlowData * MainOutput() override { return GetPinData<0>();}

protected:
    virtual bool BeforeProcessing() override;
    virtual bool ProcessInternal() override;
};

#endif // IMAGEFILTEREXAMPLE_H
