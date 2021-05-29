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

    template<int i>
    QString GetPinName()
    {
        IDataPin * pin = GetDataPinAt<i>();
        return pin->Name();
    }

    template<int i>
    FlowData * GetPinData()
    {
        IDataPin * pin = GetDataPinAt<i>();
        return pin->GetData();
    }

    template<int i>
    void SetPinData(FlowData * data)
    {
        IDataPin * pin = GetDataPinAt<i>();
        pin->SetData( data);
    }

    template<int i>
    IDataPin * GetDataPinAt()
    {
        typedef typename TypeAt<GenericPinTypeList,i>::Result TypeAtIndex;
        GenericNodePinHolder<TypeAtIndex> pinHolder = Field<i, PinHierarchy>(_pins);
        IDataPin * pin = pinHolder._genericPin;
        return pin;
    }

    PinHierarchy Pins() { return _pins;}

protected:
    PinHierarchy _pins;
    //T2 _nodeProcessor;
};

#endif // GENERICNODE_H
