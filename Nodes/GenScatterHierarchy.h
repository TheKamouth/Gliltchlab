#ifndef GENSCATTERHIERARCHY_H
#define GENSCATTERHIERARCHY_H

#include "TypeList.h"

template< class TList, template < class > class Unit >
class GenScatterHierarchy;

template< class Head, class Tail, template < class > class Unit >
class GenScatterHierarchy< TypeList< Head, Tail >, Unit > :
    public GenScatterHierarchy< Head, Unit >,
    public GenScatterHierarchy< Tail, Unit >
{
    typedef class TypeList< Head, Tail > TList;
    typedef class GenScatterHierarchy< Head, Unit > LeftBase;
    typedef class GenScatterHierarchy< Tail, Unit > RightBase;
};

template < class AtomicType, template < class > class Unit >
class GenScatterHierarchy : public Unit< AtomicType >
{
    typedef class Unit< AtomicType > LeftBase;
};

template< template < class > class Unit >
class GenScatterHierarchy< NullType, Unit >
{

};

#endif // GENSCATTERHIERARCHY_H
