#include "GenericNodePin.h"

#include <QDebug>

template<class A, class B>
GenericNodePin<A,B>::GenericNodePin()
{
    _pinName = __FUNCTION__;
}

template<class A, class B>
void GenericNodePin<A,B>::SetData(A * data)
{
    _data = data;
}

template<class A, class B>
A *GenericNodePin<A,B>::GetData()
{
    return _data;
}

template<class A, class B>
bool GenericNodePin<A,B>::IsInput()
{
    if( std::is_same<B, InputNodePin>::value == true)
    {
        return true;
    }
    else if( std::is_same<B, OutputNodePin>::value == true)
    {
        return true;
    }
    else
    {
        qDebug() << "Second template parameter should be either InputNodePin either OutputNodePin";
    }
}

template<class A, class B>
QString GenericNodePin<A,B>::Name()
{
    return _pinName;
}

// Cannot do this with member function partial specialization for it needs to define a partial specialization of the class
// Could NodePin specializations (2: input + output) be useful ?
/*
template<class B>
bool GenericNodePin<InputNodePin, B>::IsInput()
{
    return true;
}

template<class B>
bool GenericNodePin<OutputNodePin, B>::IsInput()
{
    return false;
}
*/
