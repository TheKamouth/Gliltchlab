#ifndef GENERICNODE_H
#define GENERICNODE_H

#include "Nodes/GenScatterHierarchy.h"
#include "Nodes/GenericNodePin.h"

#include "TypeList.h"
#include "FlowGraph/NodeConnection.h"
#include "FlowGraph/FlowData.h"

#include <QImage>
#include <QDebug>

#include <memory>
#include <tuple>
#include <vector>

template<class T1>
struct GenericNodePinHolder
{
    T1 * _genericPin;
};

// T1 is a GenericNodePin type list, data describing node pins in/out and type
// T2 is a NodeProcessor implementation
template< class T1/*, class T2*/>
class GenericNode
{
public:
    typedef std::integral_constant<int, Length< T1 >::value > PinCount;
    typedef GenScatterHierarchy< T1, GenericNodePinHolder> PinHierarchy;

    QString GetPinName(int index)
    {
        return Field<index>(_pins)._genericPin.Name();
    }

private:
    PinHierarchy _pins;
    //T2 _nodeProcessor;
};

// T1 is a GenericNode
// T2 is an index
template< class T1, int i >
GenericNodePin<class A, class B> * GetNodeGenericPinAt()
{

}

// A template function to get pin at index i
//tempalte

#endif // GENERICNODE_H
