#ifndef GENERICNODE_H
#define GENERICNODE_H

#include "Nodes/GenScatterHierarchy.h"
#include "Nodes/GenericNodePin.h"

#include "TypeList.h"
#include "FlowGraph/NodeConnection.h"

#include <QImage>
#include <QDebug>

#include <memory>
#include <tuple>
#include <vector>

template<class T1>
class GenericNodePinHolder
{
    T1 * pin;
};

// 2 pin GenericNode
template<class T1, class T2>
class GenericNode : public GenScatterHierarchy< TYPE_LIST_2(T1, T2), GenericNodePinHolder>
{};

// Error : This is a redefinition of GenericNode. Could change the name, impractical.
// 3 pin GenericNode
//template< class T1, class T2, class T3>
//class GenericNode : public GenScatterHierarchy< TYPE_LIST_3(T1, T2, T3), GenericNodePinHolder>
//{};


#endif // GENERICNODE_H
