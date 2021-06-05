#ifndef GENERICNODEHELPERS_H
#define GENERICNODEHELPERS_H

#include "Nodes/GenScatterHierarchy.h"
#include "Nodes/GenericNodePin.h"
#include "Nodes/ProcessorNode.h"
#include "Nodes/INode.h"
/*#include "Nodes/IDataPin.h"
#include "NodeFactory.h"

#include "TypeList.h"
#include "FlowGraph/NodeConnection.h"
#include "FlowGraph/FlowData.h"

#include <QImage>
#include <QDebug>

#include <memory>
#include <tuple>
#include <vector>
#include <array>

// T is a GenScatterHierarchy
template <class TList>
struct DataPinList {};

template <class T>
QList<IDataPin *> DataPinList< TypeList<T, NullType> >
{
    static std::array<IDataPin*, 1> constexpr value = {};
};

// Return a list of pointer on GenericPinHolder member
template <class T, class U>
QList<IDataPin *> DataPinList< TypeList<T,U> >()
{
    //static constexpr QList<IDataPin*> value ;//= QList<IDataPin*>().append( DataPinList<T2>::value );

    static int constexpr typeListLength = PinCount::value;
    static int constexpr currentTypeListLength = Length< TypeList< T,U > >::value;
    static int constexpr currentTypeIndex = typeListLength - currentTypeListLength;

    typedef typename TypeAt<GenericPinTypeList,currentTypeIndex>::Result TypeAtIndex;

    // this is meant to be a pointer to GenericPinHolder::_genericPin
    static constexpr IDataPin * dataPin = GetDataPinAt<currentTypeIndex>();

    GenericNodePinHolder<TypeAtIndex> & pinHolder = Field<currentTypeIndex, PinHierarchy>(_pins);

    static std::array<IDataPin*, typeListLength> constexpr value = {};
};

*/
#endif // GENERICNODEHELPERS_H
