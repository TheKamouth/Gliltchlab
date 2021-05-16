#ifndef NODECONNECTION_H
#define NODECONNECTION_H

#include "Nodes/GenericNode.h"

#include <QString>
#include <QImage>

struct ConnectionData
{
    enum Type
    {
        Int,
        Float,
        String,
        Image
        //Audio,
        //Video,
    };
    ConnectionData();
    ConnectionData(int   e) { _data.INT    = e; _type = Int;}
    ConnectionData(float e) { _data.FLOAT  = e; _type = Float;}
    ConnectionData(QString * e) { _data.STRING = e; _type = String;}
    ConnectionData(QImage * e) { _data.IMAGE = e; _type = Image;}

    Type GetType() const { return _type; }
    int GetInt() const { return _data.INT; }
    float GetFloat() const { return _data.FLOAT; }
    QString * GetString() const { return _data.STRING; }
    QImage * GetImage() const { return _data.IMAGE; }

private:
    Type _type;

    // use QVariant instead ?
    union {
        int   INT;
        float FLOAT;
        QString * STRING;
        QImage * IMAGE;
        //QAudio * AUDIO;
        //QVideo * VIDEO;
    }_data;
};


enum ConnectionDirection
{
    InputDirection,
    OutputDirection,
    None
};

enum ConnectionType
{
    SpecificWidget,
    Node,
    TimeLine
};

class Input{};
class Output{};

// ?
class INodeConnection
{
    //void * Data() { return _connectionData; }
};

// Might be a connection to another node, to timeline or to a specificUI widget (default)
// This should handle data in specific way
// This should not instantiate data but either:
//      - distribute it to connected node
//      - gather input node data
// Connection is 1 to 1 for now
template <class InOut, class ConnectionData>
class NodeConnection : INodeConnection
{
public:
    NodeConnection();

    // INodeConnection ?

    void SetData(ConnectionData * data){ _connectionData = data; }
    ConnectionData * Data() { return _connectionData; }
    QString Name() { return "unnamed connection"; }

    // This should read (if input) or write (if output)
    void Refresh();

private:
    // fields
    QString _connectionName;
    ConnectionData * _connectionData;
    ConnectionType _connectionType;
    ConnectionDirection _connectionDirection;

    // This should not be a type dependant on template arguments
    // GenericNode<> * _connectedNode;

    // methods
    ConnectionDirection Direction();
};
#endif // NODECONNECTION_H
