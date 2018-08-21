#ifndef INNERTYPE_CALC
#define INNERTYPE_CALC

#include "void_t.hpp"
#include "unique_type.hpp"

template<typename T, class = void>
struct has_value_type : std::false_type {};

template<typename T>
struct has_value_type<T,void_t<typename T::value_type> > : std::true_type {};

//hides value type instantiation
template<typename T, class = void>
struct hide_VT_inst
{
    typedef T type;
};

template<typename T>
struct hide_VT_inst<T, void_t<typename T::value_type> >
{
    typedef typename T::value_type type;
};

//pass in one-arg template & type, returns if type is template specialization or not.
//REQUIRES member typedef value_type as inner type of templ_type. Indentation organized
//so that multiline closing and opening brackets are vertically aligned
template<template<typename> typename templ_type, typename outertype>
using is_VTtemplate_of_1 =
typename std::is_same<outertype, templ_type<
               typename std::conditional<
               !std::is_integral<outertype>::value && has_value_type<outertype>::value,
               typename hide_VT_inst<outertype>::type,
                              unique_type>
                                      ::type>
                      >::type;

template<typename T, class = void>
struct has_Ltype : std::false_type {};

template<typename T>
struct has_Ltype<T, void_t<typename T::Ltype> > : std::true_type {};

template<typename T, class = void>
struct has_Rtype : std::false_type {};

template<typename T>
struct has_Rtype<T, void_t<typename T::Rtype> > : std::true_type {};

template<typename T, class = void>
struct hide_Ltype_inst
{
    typedef T type;
};

template<typename T>
struct hide_Ltype_inst<T, void_t<typename T::Ltype> >
{
    typedef typename T::Ltype type;
};

template<typename T, class = void>
struct hide_Rtype_inst
{
    typedef T type;
};

template<typename T>
struct hide_Rtype_inst<T, void_t<typename T::Rtype> >
{
    typedef typename T::Rtype type;
};

class op_type;

//pass in type return std::true_type/std::false_type
template<typename outertype>
using is_generic_op_tree =
typename std::is_same<outertype, op_tree<
            typename std::conditional<
has_Ltype<outertype>::value,
typename hide_Ltype_inst<outertype>::type,
unique_type                           >::type,

            typename std::conditional<
has_Rtype<outertype>::value,
typename hide_Rtype_inst<outertype>::type,
unique_type
                                      >::type
                                         >
                      >::type;

#endif // INNERTYPE_CALC