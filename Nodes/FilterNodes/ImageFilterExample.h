#ifndef IMAGEFILTEREXAMPLE_H
#define IMAGEFILTEREXAMPLE_H

#include "Nodes/Node.h"
#include "Nodes/GenericNode.h"
#include "Nodes/GenericNodePin.h"
#include "Nodes/GenScatterHierarchy.h"
#include "Nodes/TypeList.h"
#include "Nodes/IDataPin.h"

typedef TYPE_LIST_3(ImageInputPin, ImageOutputPin, IntInputPin) ImageFilterExamplePinsTlist ;

class ImageFilterNodeExample : public GenericNode<ImageFilterExamplePinsTlist>
{

public:
    ~ImageFilterNodeExample(){};

    // INode
    virtual NodeType Type() override { return ImageFilterExample;}
    virtual FlowData * MainOutput() override { return GetPinData<0>();}

protected:
    virtual bool BeforeProcessing() override;
    virtual bool ProcessInternal() override;
};

#endif // IMAGEFILTEREXAMPLE_H
