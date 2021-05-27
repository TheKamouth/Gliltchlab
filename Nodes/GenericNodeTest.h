#ifndef GENERICNODETEST_H
#define GENERICNODETEST_H

#include "GenScatterHierarchy.h"
#include "GenericNode.h"
#include "GenericNodePin.h"
#include "FlowGraph/NodeConnection.h"

#include <QImage>
#include <QDebug>

/*
class GenericNodeImplTest : public GenericNode<
        GenericNodePin< QImage *, InputNodePin>//,
        //NodeConnection< QImage * , OutputNodePin>
        >
{};


template< class T >
struct Holder
{
    T * data;
};
*/

// defining a node holding a QImage* and an int as member
//typedef GenScatterHierarchy< TYPE_LIST_2(QImage *, int), GenericNodePinHolder> NodeType;
//typedef GenScatterHierarchy< TYPE_LIST_2(ImageInputPin, ImageOutputPin), GenericNodePinHolder> NodeDataType2;
//typedef GenScatterHierarchy< TYPE_LIST_3(IntInputPin, ImageInputPin, ImageOutputPin), GenericNodePinHolder> NodeDataType3;

//typedef GenScatterHierarchy< TYPE_LIST_2(GenericNodePin< QImage *, InputNodePin>,GenericNodePin< QImage *, OutputNodePin>), >;


// A list of interface value type is defined
//template <typename ... Types> struct TypesList {};
//using HandledConnectionTypesList = TypesList<int, float, QImage*>;

// This is a easy expandable base class for Nodes
// T is the output provided interface type
/*
 * template <class Output>
class GenericNode
{
public:
    GenericNode();

private:
    Output * _output;
};
*/

// Handling input and output the same way would be better
// <=> a Node implementation can have * input and * output
/*

class ImageInputNode : GenericNode<QImage*>

ou plutot

class ImageInputNode :  GenericNode<
                        GenericNodePin< Input, QImage * >
                        >

class ImageFilterNode : GenericNode<
                        GenericNodePin< Input, QImage * >
                        GenericNodePin< Output, QImage * >
                        >

class ImageFilterNode : GenericNode<
                        GenericNodePin< Input, QImage * >
                        GenericNodePin< Output, QImage * >
                        >

class ImageFilterNode : GenericNode<
                        GenericNodePin< Input, QImage * >
                        GenericNodePin< Output, QImage * >
                        // Parameter
                        GenericNodePin< Input, float>
                        >
*/

#endif // GENERICNODETEST_H
