#ifndef GENERICNODETEST_H
#define GENERICNODETEST_H

#include "GenericNode.h"
#include "FlowGraph/NodeConnection.h"

#include <QImage>
#include <QDebug>

class GenericNodeImplTest : public GenericNode<
        NodeConnection< Input, QImage*>,
        NodeConnection< Output, QImage*>
        >
{

};


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
                        Connection< Input, QImage * >
                        >

class ImageFilterNode : GenericNode<
                        Connection< Input, QImage * >
                        Connection< Output, QImage * >
                        >

class ImageFilterNode : GenericNode<
                        Connection< Input, QImage * >
                        Connection< Output, QImage * >
                        >

class ImageFilterNode : GenericNode<
                        Connection< Input, QImage * >
                        Connection< Output, QImage * >
                        // Parameter
                        Connection< Input, float>
                        >
*/

#endif // GENERICNODETEST_H
