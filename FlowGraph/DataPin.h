#ifndef DATAPIN_H
#define DATAPIN_H

#include "FlowGraph/FlowData.h"

#include <QDebug>

// A node should not be able to modify its input
// It also should not be able to read its output
/*
class IDataPin
{
public:
    virtual void SetData(FlowData * data) = 0;
    virtual FlowData * GetData() = 0;
    virtual bool IsInput() = 0;
    virtual QString Name() { return "unamed pin";}

protected:
    FlowData * _flowData;
};

class IOutputDataPin : public IDataPin
{
public:
    virtual void SetData(FlowData * data) override final { _flowData = data;};
    virtual FlowData * GetData() override final{ return nullptr;}
    virtual bool IsInput() override final { return false; };
};

template <typename T>
class IInputDataPin : public IDataPin
{
public:
    //IInputDataPin(){_flowData->SetType = T}
    virtual void SetData(FlowData * data) override final { qDebug() << "Warning : Setting data on an input node. data: "; Q_UNUSED(data);};
    virtual FlowData * GetData() override final{ return _flowData;}
    virtual bool IsInput() override final { return true; };
};

*/
#endif // DATAPIN_H
