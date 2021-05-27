#ifndef GENERICNODE_H
#define GENERICNODE_H

#include "FlowGraph/NodeConnection.h"

#include <QImage>
#include <QDebug>

#include <memory>
#include <tuple>
#include <vector>

// A list of interface value type is defined
template <typename ... Types> struct TypesList {};
using HandledConnectionTypesList = TypesList<int, float, QImage*>;

// GenericNode is responsible for its output data (it news and deletes it)
// Every pointer on input data should be nullify :
//  - when input get disconnected/deleted
//  - when this node is deleted
template <class ... Connections>
class GenericNode
{
public:

    // recursion end
    template< class T>
    GenericNode()
    : _connections()
    {
    };

    void Initialize();

    int ConnectionCount();
    void InitNodeConnectionData();

    // Return a type of type class Connections'index'
    // Type returned is variable
    template <class ConnectionDataType>
    ConnectionDataType * NodeConnectionData(int index);

    // Using an interface ?
    //INodeConnection * NodeConnection(int index);

    std::tuple< std::vector<Connections>...> ConnectionsTuple()
    {
        return _connections;
    }

    template<class C>
    void Initialize(C && T ...);

    //template< class ... C>
    //template< class T>
    //void Initialize(std::vector<T> &vec, C &&c1, C... &&cExpanded);

private:
     std::tuple< std::vector<Connections>... > _connections;
    // This list is not required : Connections objects data/name/direction are known at compile time
    // QVector<Connection *> _connections;
};

#endif // GENERICNODE_H
