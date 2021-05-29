#ifndef GENERICNODE_H
#define GENERICNODE_H

#include "Nodes/GenScatterHierarchy.h"
#include "Nodes/GenericNodePin.h"
#include "Nodes/ProcessorNode.h"
#include "Nodes/INode.h"
#include "NodeFactory.h"

#include "TypeList.h"
#include "FlowGraph/NodeConnection.h"
#include "FlowGraph/FlowData.h"

#include <QImage>
#include <QDebug>

#include <memory>
#include <tuple>
#include <vector>

template<class T1>
struct GenericNodePinHolder
{
    //GenericNodePinHolder(){test = 1;_genericPin = new T1();}
    //GenericNodePinHolder(T1 * t){test = 0; _genericPin = t;}
    T1 * _genericPin;
    int test;
};

// T1 is a GenericNodePin type list, data describing node pins in/out and type
// T2 is a NodeProcessor implementation
template< class T1/*, class T2*/>
class GenericNode : public INode
{
public:
    GenericNode() : _flowGraphScenePosition(QPointF(0.0f,0.0f))
    {
        /*int pinCount = GetPinCount();
        for( int i = 0; i < pinCount; i++)
        {
            IDataPin * dataPin = GetDataPinAt(i);
            //dataPin =
        }*/
    }
    virtual ~GenericNode(){};

    typedef T1 GenericPinTypeList;
    typedef std::integral_constant<int, Length< GenericPinTypeList >::value > PinCount;
    typedef GenScatterHierarchy< GenericPinTypeList, GenericNodePinHolder> PinHierarchy;

    template<int i>
    QString GetPinName()
    {
        IDataPin * pin = GetDataPinAt<i>();
        return pin->Name();
    }

    template<int i>
    FlowData * GetPinData()
    {
        IDataPin * pin = GetDataPinAt<i>();
        return pin->GetData();
    }

    template<int i>
    void SetPinData(FlowData * data)
    {
        IDataPin * pin = GetDataPinAt<i>();
        pin->SetData( data);

        emit NodeInputChanged(this);
    }

    template<int i>
    IDataPin * GetDataPinAt()
    {
        typedef typename TypeAt<GenericPinTypeList,i>::Result TypeAtIndex;
        GenericNodePinHolder<TypeAtIndex> pinHolder = Field<i, PinHierarchy>(_pins);
        //IDataPin pin = pinHolder._genericPin;
        return pinHolder._genericPin;
    }

    virtual IDataPin * GetDataPinAt(int index) override
    {
        // This could be better
        switch(index)
        {
            case 0:
                return GetDataPinAt<0>();
            case 1:
                return GetDataPinAt<1>();
            case 2:
                return GetDataPinAt<2>();
           /* case 3:
                return GetDataPinAt<3>();
            case 4:
                return GetDataPinAt<4>();
            case 5:
                return GetDataPinAt<5>();*/
            default:
                qDebug() << "maximum 6 pin handled";
                return GetDataPinAt<0>();
        }
    }

    PinHierarchy Pins() { return _pins;}
    virtual QString Name() {return "GenericNode"; };
    virtual NodeType Type() = 0;
    QPointF FlowGraphNodePosition(){ return _flowGraphScenePosition; }
    void SetFlowGraphScenePosition(QPointF graphScenePosition){ _flowGraphScenePosition = graphScenePosition; }

    virtual int GetPinCount() override { return PinCount::value; }
    virtual int GetMaxInOutPin() override { return PinCount::value; }

protected:
    PinHierarchy _pins;
    QPointF _flowGraphScenePosition;
};

#endif // GENERICNODE_H
