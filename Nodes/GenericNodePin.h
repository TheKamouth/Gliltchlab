#ifndef GENERICNODEPIN_H
#define GENERICNODEPIN_H

#include "FlowGraph/FlowData.h"
#include "FlowGraph/DataPin.h"

#include <QString>
#include <QImage>

#include <type_traits>
#include <string>

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
    GenericNodePin();

    bool IsInput();
    QString Name();

    static const QString _pinName;
};

template <class A>
class GenericNodePin<A, InputNodePin> : public IInputDataPin
{
public:
    GenericNodePin();
};

template <class A>
class GenericNodePin<A, OutputNodePin> : public IOutputDataPin
{
public:
    GenericNodePin();
};

typedef class GenericNodePin< int, InputNodePin > IntInputPin ;
typedef class GenericNodePin< int, OutputNodePin > IntOutputPin ;
typedef class GenericNodePin< float, InputNodePin > FloatInputPin ;
typedef class GenericNodePin< float, OutputNodePin > FloatOutputPin ;
typedef class GenericNodePin< QImage *, InputNodePin > ImageInputPin;
typedef class GenericNodePin< QImage *, OutputNodePin > ImageOutputPin ;

#endif // GENERICNODEPIN_H
