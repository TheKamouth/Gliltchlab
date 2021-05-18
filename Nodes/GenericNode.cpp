#include "GenericNode.h"

//template< class ... Connections >
//GenericNode< Connections ... connections>::GenericNode(Connections *) : _connections{connections ...}
//{

//}

// arbitrary diff for branching test purpose

template< class ... Connections>
int GenericNode<Connections...>::ConnectionCount()
{
    const std::size_t n = sizeof...(Connections);
    return n;
}

template<class ... Connections>
void GenericNode<Connections ...>::InitNodeConnectionData()
{
    //std::vector<ConnectionData> vector = {Connections...};
    for(int i = 0 ; i < ConnectionCount(); i++)
    {
        //type = typename std::tuple_element<i, _connections>::type;

    }
}

/*
template< class ... Connections>
INodeConnection * GenericNode<Connections ...>::NodeConnection(int index)
{
    if( index < 0 || index > ConnectionCount())
    {
        qDebug() << "invalid node connection index : " << index;
        return nullptr;
    }

    //std::vector<Connections...> vector = std::get<index>(_connections);
    //return &(INodeConnection)vector[0] ;

    return nullptr;
}
*/

template< class ... Connections>
template< class ConnectionDataType>
ConnectionDataType * GenericNode<Connections ...>::NodeConnectionData(int index)
{
    if( index < 0 || index > ConnectionCount())
    {
        qDebug() << "invalid node connection index : " << index;
        return nullptr;
    }


    std::vector<Connections...> vector = std::get<index>(_connections);
    //ConnectionDataType * data = vector[0].;
    //bool isInputConnection =

    return nullptr;
}
