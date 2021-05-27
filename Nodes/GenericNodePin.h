#ifndef GENERICNODEPIN_H
#define GENERICNODEPIN_H

#include <QString>
#include <QImage>

#include <type_traits>
#include <string>

// NodePin is either input or output
template<bool IsInput>
class IsInputNodePin{};

typedef IsInputNodePin<true> InputNodePin;
typedef IsInputNodePin<false> OutputNodePin;

// A is the type of data handled by the pin
// B is InputNodePin or OutputNodePin (how to enforce it ?)
template <class A, class B>
class GenericNodePin
{
public:
    GenericNodePin();

    void SetData(A * data);
    A * GetData();
    bool IsInput();
    QString Name();

private:
    A * _data;

    static const QString _pinName;
};

typedef class GenericNodePin< int, InputNodePin > IntInputPin ;
typedef class GenericNodePin< int, OutputNodePin > IntOutputPin ;
typedef class GenericNodePin< QImage *, InputNodePin > ImageInputPin;
typedef class GenericNodePin< QImage *, OutputNodePin > ImageOutputPin ;

#endif // GENERICNODEPIN_H
