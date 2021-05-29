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
    typedef T1 GenericPinTypeList;
    typedef std::integral_constant<int, Length< GenericPinTypeList >::value > PinCount;
    typedef GenScatterHierarchy< GenericPinTypeList, GenericNodePinHolder> PinHierarchy;

    QString GetPinName(int index)
    {
        IDataPin * pin = Field<0>(_pins)._genericPin;
        return pin->Name();
    }

    /*
    IDataPin * GetDataPinAt(int index)
    {
        IDataPin * pin = Field<index>(_pins)._genericPin;
        return pin;
    }*/

private:
    PinHierarchy _pins;
    //T2 _nodeProcessor;
};

// T1 is a GenericNode
// T2 is an index
/*
template< class T1, int i >
GenericNodePin<class A, class B> * GetNodeGenericPinAt(T1 * genericNode)
{
    // TODO
    typedef TypeAt< T1::GenericPinTypeList >
    GenericNodePin<A,B> * pin = Field<i>(genericNode, i)._genericPin;
    return pin;
}
*/
#endif // GENERICNODE_H
