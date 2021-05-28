// Andrei Alexandrescu's widsom from Modern C++ design
#ifndef TYPELIST_H
#define TYPELIST_H

#define TYPE_LIST_1(T1)             TypeList< T1, NullType>
#define TYPE_LIST_2(T1, T2)         TypeList< T1, TYPE_LIST_1(T2)>
#define TYPE_LIST_3(T1, T2, T3)     TypeList< T1, TYPE_LIST_2(T2, T3)>

template <class T, class U>
class TypeList
{
    typedef T Head;
    typedef U Tail;
};

class NullType{};

// Length
template <class TList>
struct Length;

template <>
struct Length<NullType>
{
    enum { value = 0};
};

template <class T1, class T2>
struct Length< TypeList< T1,T2> >
{
    enum { value = 1 + Length<T1>::value };
};

template <int v>
struct IntToType
{
    enum
    {
        value = v
    };
};

template <class T>
struct TypeToType
{
    typedef T OriginalType;
};

/*
template< class H, class R >
inline R& FieldHelper( H & obj, TypeToType<R>, IntToType<0>)
{
    typename H::LeftBase & subObj = obj;
    return subObj;
}

template< class H, class R, int i>
inline R& FieldHelper( H & obj, TypeToType<R> tt, IntToType<i>)
{
    typename H::RightBase & subObj = obj;
    return FieldHelper( subObj, tt, IntToType<i - 1>());
}

// defining return type in template function definition ?! :o
template< int i, class H >
typename Private::FieldTraits<H>::At<i>::Result Result&
Field( H & obj)
{
    typedef typename Private::FieldTraits<H>::At<i>::Result Result;
    return FieldHelper( obj, TypeToType<Result>(), IntToType<i>());
}
*/
#endif // TYPELIST_H
