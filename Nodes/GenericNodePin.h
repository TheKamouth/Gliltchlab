#ifndef GENERICNODEPIN_H
#define GENERICNODEPIN_H

#include "FlowGraph/FlowData.h"
#include "FlowGraph/DataPin.h"

#include <QString>
#include <QImage>

#include <type_traits>
#include <string>

// A node should not be able to modify its input
// It also should not be able to read its output

class IDataPin
{
public:
    IDataPin() { _flowData = new FlowData(); _flowData->SetType(Int); }
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

typedef IInputDataPin<QImage *> InputImagePin;

typedef class IInputDataPin<int> IntInputPin ;
typedef class IOutputDataPin<int> IntOutputPin ;
typedef class IInputDataPin<QImage *> ImageInputPin ;
typedef class IOutputDataPin<QImage *> ImageOutputPin ;

// Below is the first Pin classes
// NodePin is either input or output
template<bool IsInput>
class IsInputNodePin{};

typedef IsInputNodePin<true> InputNodePin;
typedef IsInputNodePin<false> OutputNodePin;

// TODO : GenericNodePin template argument must include a unique identifier
// because GenericNodePin are defined by the node implementation, and a node can have several in/output of the same type

// A is the type of data handled by the pin
// B is InputNodePin or OutputNodePin (how to enforce it ?)
// T3 is the pin name, used by NodeInOutPins::operator[]
template <class A, class B/*, class T3*/>
class GenericNodePin
{
public:
    GenericNodePin(){_pinName = __FUNCTION__;}

    bool IsInput();
    QString Name();

    static const QString _pinName;
};

template <class A>
class GenericNodePin<A, InputNodePin> : public IInputDataPin<A>
{
public:
    GenericNodePin();
};

template <class A>
class GenericNodePin<A, OutputNodePin> : public IOutputDataPin<A>
{
public:
    GenericNodePin();
};

/*
typedef class GenericNodePin< int, InputNodePin > IntInputPin ;
typedef class GenericNodePin< int, OutputNodePin > IntOutputPin ;
typedef class GenericNodePin< float, InputNodePin > FloatInputPin ;
typedef class GenericNodePin< float, OutputNodePin > FloatOutputPin ;
typedef class GenericNodePin< QImage *, InputNodePin > ImageInputPin;
typedef class GenericNodePin< QImage *, OutputNodePin > ImageOutputPin ;
typedef class GenericNodePin< QString, InputNodePin > StringInputPin ;
typedef class GenericNodePin< QString, OutputNodePin > StringOutputPin ;
*/

#endif // GENERICNODEPIN_H
