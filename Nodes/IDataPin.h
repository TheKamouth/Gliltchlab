#ifndef IDATAPIN_H
#define IDATAPIN_H

#include "FlowGraph/FlowData.h"
#include "FlowGraph/DataPin.h"

#include <QString>
#include <QImage>

#include <type_traits>
#include <string>

class INode;

// This is the interface for GenericNode<> impletentions
// IDataPin are not responsible for _flowData, the node on "output side" of the connection
// (ie the one providing the data) is responsible for it
class IDataPin
{
public:
    IDataPin(INode * node = nullptr) :
        _hostNode(node),
        _connectedPin(nullptr)
    {
        // Default FlowData for now, should be removed (see above comment)
        _flowData = new FlowData();
        _flowData->SetType(Int);
        _isConnected = false;
    }

    FlowDataType Type()
    {
        return _flowData->GetType();
    }
    virtual void SetData(FlowData * data) = 0;
    virtual FlowData * GetData() = 0;
    virtual bool IsInput() = 0;
    virtual QString Name()
    {
        FlowData * data = GetData();
        QString typeString = data->TypeString(_flowData->GetType());
        return typeString;
    }

    bool IsConnected()
    {
        return _isConnected;
    }
    void SetConnectedPin(IDataPin * pin)
    {
        _isConnected = true;
        _connectedPin = pin;
    }
    IDataPin * ConnectedPin()
    {
        return _connectedPin;
    }
    INode * Node()
    {
        return _hostNode;
    }

protected:
    INode * _hostNode;
    IDataPin * _connectedPin;
    FlowData * _flowData;
    bool _isConnected;
};

// IOutputDataPin and IInputDataPin
template <typename T>
class IOutputDataPin : public IDataPin
{
public:
    IOutputDataPin() { _flowData = new FlowData(); }
    virtual void SetData(FlowData * data) override final { _flowData = data;};
    virtual FlowData * GetData() override final{ return _flowData;}
    virtual bool IsInput() override final { return false; };
};

template <typename T>
class IInputDataPin : public IDataPin
{
public:
    virtual void SetData(FlowData * data) override final { qDebug() << "Warning : Setting data on an input node. data: "; Q_UNUSED(data);};
    virtual FlowData * GetData() override final{ return _flowData;}
    virtual bool IsInput() override final { return true; };
};

// There must be a better way
// IInputDataPin specializations
template <>
class IInputDataPin<int> : public IDataPin
{
public:
    IInputDataPin() { _flowData = new FlowData(); _flowData->SetType(Int);}
    virtual void SetData(FlowData * data) override final { qDebug() << "Warning : Setting data on an input node. data: "; Q_UNUSED(data);};
    virtual FlowData * GetData() override final{ return _flowData;}
    virtual bool IsInput() override final { return true; };
};

template <>
class IInputDataPin<QString> : public IDataPin
{
public:
    IInputDataPin() { _flowData = new FlowData(); _flowData->SetType(String);}
    virtual void SetData(FlowData * data) override final { qDebug() << "Warning : Setting data on an input node. data: "; Q_UNUSED(data);};
    virtual FlowData * GetData() override final{ return _flowData;}
    virtual bool IsInput() override final { return true; };
};

template <>
class IInputDataPin<QImage *> : public IDataPin
{
public:
    IInputDataPin() { _flowData = new FlowData(); _flowData->SetType(Image);}
    virtual void SetData(FlowData * data) override final { qDebug() << "Warning : Setting data on an input node. data: "; Q_UNUSED(data);};
    virtual FlowData * GetData() override final{ return _flowData;}
    virtual bool IsInput() override final { return true; };
};

template <>
class IInputDataPin<float> : public IDataPin
{
public:
    IInputDataPin() { _flowData = new FlowData(); _flowData->SetType(Float);}
    virtual void SetData(FlowData * data) override final { qDebug() << "Warning : Setting data on an input node. data: "; Q_UNUSED(data);};
    virtual FlowData * GetData() override final{ return _flowData;}
    virtual bool IsInput() override final { return true; };
};

// There must be a better way
// IOutputDataPin specializations
template <>
class IOutputDataPin<QImage *> : public IDataPin
{
public:
    IOutputDataPin() { _flowData = new FlowData(); _flowData->SetType(Image);}
    virtual void SetData(FlowData * data) override final { _flowData = data;};
    virtual FlowData * GetData() override final{ return _flowData;}
    virtual bool IsInput() override final { return false; };
};

template <>
class IOutputDataPin<float> : public IDataPin
{
public:
    IOutputDataPin() { _flowData = new FlowData(); _flowData->SetType(Float);}
    virtual void SetData(FlowData * data) override final { _flowData = data;};
    virtual FlowData * GetData() override final{ return _flowData;}
    virtual bool IsInput() override final { return false; };
};

typedef class IInputDataPin<int> IntInputPin ;
typedef class IOutputDataPin<int> IntOutputPin ;
typedef class IInputDataPin<float> FloatInputPin ;
typedef class IOutputDataPin<float> FloatOutputPin ;
typedef class IInputDataPin<QString> StringInputPin ;
typedef class IInputDataPin<QImage *> ImageInputPin ;
typedef class IOutputDataPin<QImage *> ImageOutputPin ;
#endif // IDATAPIN_H
