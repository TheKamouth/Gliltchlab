#ifndef IDATAPIN_H
#define IDATAPIN_H

#include "FlowGraph/FlowData.h"
#include "FlowGraph/DataPin.h"

#include <QString>
#include <QImage>

#include <type_traits>
#include <string>

// This is the interface for GenericNode<> impletentions
// IDataPin are not responsible for _flowData, the node on "output side" of the connection
// (ie the one providing the data) is responsible for it
class IDataPin
{
public:
    IDataPin()
    {
        // Default FlowData for now, should be removed (see above comment)
        _flowData = new FlowData();
        _flowData->SetType(Int);
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
        QString typeString = data->TypeString();
        return typeString;
    }

protected:
    FlowData * _flowData;
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

// IInputDataPin specialization
// There must be a better way
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
class IOutputDataPin<QImage *> : public IDataPin
{
public:
    IOutputDataPin() { _flowData = new FlowData(); _flowData->SetType(Image);}
    virtual void SetData(FlowData * data) override final { _flowData = data;};
    virtual FlowData * GetData() override final{ return _flowData;}
    virtual bool IsInput() override final { return false; };
};

typedef class IInputDataPin<int> IntInputPin ;
typedef class IOutputDataPin<int> IntOutputPin ;
typedef class IInputDataPin<QString> StringInputPin ;
typedef class IInputDataPin<QImage *> ImageInputPin ;
typedef class IOutputDataPin<QImage *> ImageOutputPin ;
#endif // IDATAPIN_H
