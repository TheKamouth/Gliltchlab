#include "NodeConnection.h"

#include <QDebug>

template<class InOut, class ConnectionDataType>
void NodeConnection<InOut, ConnectionDataType>::Refresh()
{
    if(_connectionDirection == InputDirection)
    {
        // read connecteNode or listening to data changes

    }
    else if(_connectionDirection == OutputDirection)
    {
        // notify connecteNode that connection data has changed
    }
}

template<class InOut, class ConnectionDataType>
NodeConnection<InOut, ConnectionDataType>::NodeConnection()
{
    _connectionName = "unnamed connection";
    _connectionType = SpecificWidget;
    _connectionDirection = Direction();

    if( _connectionDirection == OutputDirection)
    {
        _connectionData = new FlowData();
    }
    else
    {
        _connectionData = nullptr;
    }
}

template<class InOut, class ConnectionDataType>
ConnectionDirection NodeConnection<InOut, ConnectionDataType>::Direction()
{
    bool isInput = std::is_same< InOut, Input>::value;
    bool isOutput = std::is_same< InOut, Output>::value;

    if( isInput)
    {
        return InputDirection;
    }
    else if( isOutput)
    {
        return OutputDirection;
    }
    else
    {
        qDebug() << "This should not happen." ;
        return None;
    }
}
