#ifndef RESTRICTION_CALC
#define RESTRICTION_CALC

#include "var.hpp"

//if arg is type known, return long double. Else, return original type
template<typename T>
using known_to_LD = typename std::conditional<std::is_same<T, known>::value, long double, T>::type;

//Evals to op_tree if one operand is unknown or op_tree, else long double
template<typename firsttype, typename secondtype>
using operator_ret_type = typename std::conditional<
    std::is_same<firsttype ,const unknown>::value ||
    std::is_same<secondtype,const unknown>::value ||
    is_generic_op_tree<firsttype >::value         ||
    is_generic_op_tree<secondtype>::value,
op_tree<known_to_LD<firsttype>,known_to_LD<secondtype> >, long double
                                                    >::type;

//SFINAE comparison cast of is_arithmetic. Takes in type, returns
//itself if numtype, else unique_type
template<typename T>
using is_numtype = typename std::conditional<std::is_arithmetic<T>::value,T,unique_type>::type;

template<typename T>
using is_numtype_or_op_generic = typename std::conditional<is_generic_op_tree<T>::value, T, is_numtype<T> >::type;

//restricts input for operators within equation
template<typename T> struct OPER_restrictor          {typedef is_numtype_or_op_generic<T> type;};
template<>           struct OPER_restrictor<known>   {typedef known                       type;};
template<>           struct OPER_restrictor<unknown> {typedef unknown                     type;};

//If all types valid, return type is as decided by operator_ret_type<...>.
//Else, drop from overload list.
template<typename firsttype, typename secondtype>
using OPER_overload_resolve = typename std::enable_if<
        std::is_same<firsttype , typename OPER_restrictor<firsttype >::type>::value &&
        std::is_same<secondtype, typename OPER_restrictor<secondtype>::type>::value ,
              operator_ret_type<firsttype,secondtype> >::type;

#endif // RESTRICTION_CALC
