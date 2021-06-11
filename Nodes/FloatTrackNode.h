#ifndef FLOATTRACKNODE_H
#define FLOATTRACKNODE_H

#include "Nodes/Node.h"
#include "Nodes/GenericNode.h"
#include "Nodes/GenericNodePin.h"
#include "Nodes/GenScatterHierarchy.h"
#include "Nodes/TypeList.h"
#include "Nodes/IDataPin.h"

#include "Timeline/Track.h"

typedef TYPE_LIST_2(IntInputPin,FloatOutputPin) FloatTrackPinsTlist ;

class FloatTrackNode : public GenericNode<FloatTrackPinsTlist>
{
public:
    FloatTrackNode();

    // INode
    virtual NodeType Type() override { return FloatTrackOutput;}
    virtual FlowData * MainOutput() override { return GetPinData<1>();}

    void SetAssociatedTrack(Track * track);

protected:
    virtual bool BeforeProcessing() override;
    virtual bool ProcessInternal() override;

private :
    Track * _track;
};

#endif // FLOATTRACKNODE_H
