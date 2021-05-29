#ifndef GENSCATTERHIERARCHY_H
#define GENSCATTERHIERARCHY_H

#include "TypeList.h"
#include "TypeManip.h"
#include "TypeTraits.h"

template< class TList, template < class > class Unit >
class GenScatterHierarchy;

template< class Head, class Tail, template < class > class Unit >
class GenScatterHierarchy< TypeList< Head, Tail >, Unit > :
        public GenScatterHierarchy< Head, Unit >,
        public GenScatterHierarchy< Tail, Unit >
{
public:
    typedef class TypeList< Head, Tail > TList;
    typedef class GenScatterHierarchy< Head, Unit > LeftBase;
    typedef class GenScatterHierarchy< Tail, Unit > RightBase;
    template <typename T> struct Rebind
    {
        typedef Unit<T> Result;
    };
};

template < class AtomicType, template < class > class Unit >
class GenScatterHierarchy : public Unit< AtomicType >
{
    typedef class Unit< AtomicType > LeftBase;

    template <typename T> struct Rebind
    {
        typedef Unit<T> Result;
    };
};

template< template < class > class Unit >
class GenScatterHierarchy< NullType, Unit >
{
    template <typename T> struct Rebind
    {
        typedef Unit<T> Result;
    };
};

// function template Field
// Accesses a field in an object of a type generated with GenScatterHierarchy
// Invocation (obj is an object of a type H generated with GenScatterHierarchy,
//     T is a type in the typelist used to generate H):
// Field<T>(obj)
// returns a reference to Unit<T>, where Unit is the template used to generate H

template <class T, class H>
typename H::template Rebind<T>::Result& Field(H& obj)
{
    return obj;
}

template <class T, class H>
const typename H::template Rebind<T>::Result& Field(const H& obj)
{
    return obj;
}

// function template TupleUnit
// The building block of tuples

template <class T>
struct TupleUnit
{
    T value_;
    operator T&() { return value_; }
    operator const T&() const { return value_; }
};

// class template Tuple
// Implements a tuple class that holds a number of values and provides field
//     access to them via the Field function (below)

template <class TList>
struct Tuple : public GenScatterHierarchy<TList, TupleUnit>
{
};

// helper class template FieldHelper
// See Field below

template <class H, unsigned int i> struct FieldHelper;

template <class H>
struct FieldHelper<H, 0>
{
    typedef typename H::TList::Head ElementType;
    typedef typename H::template Rebind<ElementType>::Result UnitType;

    enum
    {
        isTuple = Conversion<UnitType, TupleUnit<ElementType> >::sameType,
        isConst = TypeTraits<H>::isConst
    };

    typedef const typename H::LeftBase ConstLeftBase;

    typedef typename Select<isConst, ConstLeftBase,
    typename H::LeftBase>::Result LeftBase;

    typedef typename Select<isTuple, ElementType,
    UnitType>::Result UnqualifiedResultType;

    typedef typename Select<isConst, const UnqualifiedResultType,
    UnqualifiedResultType>::Result ResultType;

    static ResultType& Do(H& obj)
    {
        LeftBase& leftBase = obj;
        return leftBase;
    }
};

template <class H, unsigned int i>
struct FieldHelper
{
    typedef typename TypeAt<typename H::TList, i>::Result ElementType;
    typedef typename H::template Rebind<ElementType>::Result UnitType;

    enum
    {
        isTuple = Conversion<UnitType, TupleUnit<ElementType> >::sameType,
        isConst = TypeTraits<H>::isConst
    };

    typedef const typename H::RightBase ConstRightBase;

    typedef typename Select<isConst, ConstRightBase,
    typename H::RightBase>::Result RightBase;

    typedef typename Select<isTuple, ElementType,
    UnitType>::Result UnqualifiedResultType;

    typedef typename Select<isConst, const UnqualifiedResultType,
    UnqualifiedResultType>::Result ResultType;

    static ResultType& Do(H& obj)
    {
        RightBase& rightBase = obj;
        return FieldHelper<RightBase, i - 1>::Do(rightBase);
    }
};

template <int i, class H>
typename FieldHelper<H, i>::ResultType&
Field(H& obj)
{
    return FieldHelper<H, i>::Do(obj);
}

/*
template <int i, class H>
const typename FieldHelper<H, i>::ResultType&
Field(const H& obj)
{
    return FieldHelper<H, i>::Do(obj);
}
*/



#endif // GENSCATTERHIERARCHY_H
