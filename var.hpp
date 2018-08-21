#ifndef VAR_
#define VAR_

#include "op_tree.hpp"
#include "self_ref.hpp"
#include "innertype_calc.hpp"
#include <cmath>

#ifndef OPER
    #define OPER inline constexpr const //OPERator return type modifier
#endif // OPER

#define PERM static constexpr const

template<bool known = 0>
class var
{
public:
    constexpr var(long double inp = NAN)
    :value(inp)
    {}

    const long double value;
};

typedef var<1> known;
typedef var<0> unknown;

//if arg is type known, return long double. Else, return original type
template<typename T>
using known_to_LD = typename std::conditional<std::is_same<T, known>::value, long double, T>::type;

template<typename firsttype, typename secondtype> //cond evals to op_tree if one operand is unknown or op_tree, else long double
using operator_ret_type =    typename std::conditional<
                          std::is_same<firsttype ,const unknown>::value ||
                          std::is_same<secondtype,const unknown>::value ||
                          is_generic_op_tree<firsttype >::value         ||
                          is_generic_op_tree<secondtype>::value,
op_tree<known_to_LD<firsttype>,known_to_LD<secondtype> >, long double>::type;

template<typename T>
using is_numtype = typename std::conditional<std::is_arithmetic<T>::value,T,unique_type>::type; //SFINAE comparison cast of is_arithmetic

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

//takes in ONLY op_tree, known, unknown, numeric type as arg.
template<typename firsttype, typename secondtype>
OPER OPER_overload_resolve<firsttype,secondtype>
operator+(const firsttype& lhs, const secondtype& rhs)
{
    if(std::is_same<firsttype, typename is_numtype<firsttype>::type>::value)
    {
        if(std::is_same<secondtype, typename is_numtype<secondtype>::type>::value)
        {
            return static_cast<long double>(lhs+rhs);
        }
        if(std::is_same<secondtype, known>::value)
        {
            return static_cast<long double>(lhs+rhs.value);
        }
        if(std::is_same<secondtype, unknown>::value)
        {
            constexpr const op_tree<long double,secondtype>
            obj(OpType<firsttype,secondtype>::add, static_cast<long double>(lhs), rhs);
            return obj;
        }
        if(is_generic_op_tree<secondtype>::value)
        {
            constexpr const op_tree<long double,secondtype>
            obj(OpType<firsttype,secondtype>::add, static_cast<long double>(lhs), rhs);
            return obj;
        }
    }
    if(std::is_same<firsttype, known>::value)
    {
        if(std::is_same<secondtype, typename is_numtype<secondtype>::type>::value)
        {
            return static_cast<long double>(lhs.value+rhs);
        }
        if(std::is_same<secondtype, known>::value)
        {
            return static_cast<long double>(lhs.value+rhs.value);
        }
        if(std::is_same<secondtype, unknown>::value)
        {
            constexpr const op_tree<long double,secondtype>
            obj(OpType<long double,secondtype>::add, static_cast<long double>(lhs.value), rhs);
            return obj;
        }
        if(is_generic_op_tree<secondtype>::value)
        {
            constexpr const op_tree<long double,secondtype>
            obj(OpType<long double,secondtype>::add, static_cast<long double>(lhs.value), rhs);
            return obj;
        }
    }
    if(std::is_same<firsttype, unknown>::value)
    {
        if(std::is_same<secondtype, typename is_numtype<secondtype>::type>::value)
        {
            constexpr const op_tree<firsttype,long double>
            obj(OpType<firsttype,long double>::sub, lhs, static_cast<long double>(rhs));
            return obj;
        }
        if(std::is_same<secondtype, known>::value)
        {
            constexpr const op_tree<firsttype,long double>
            obj(OpType<firsttype,long double>::sub, lhs, static_cast<long double>(rhs));
            return obj;
        }
        if(std::is_same<secondtype, unknown>::value)
        {
            constexpr const op_tree<firsttype,secondtype>
            obj(OpType<firsttype,secondtype>::sub, lhs, rhs);
            return obj;
        }
        if(is_generic_op_tree<secondtype>::value)
        {
            constexpr const op_tree<firsttype,secondtype>
            obj(OpType<firsttype,secondtype>::sub, lhs, rhs);
            return obj;
        }
    }
    if(is_generic_op_tree<firsttype>::value)
    {
        if(std::is_same<secondtype, typename is_numtype<secondtype>::type>::value)
        {
            constexpr const op_tree<firsttype,long double>
            obj(OpType<firsttype,long double>::sub, lhs, static_cast<long double>(rhs));
            return obj;
        }
        if(std::is_same<secondtype, known>::value)
        {
            constexpr const op_tree<firsttype,long double>
            obj(OpType<firsttype,long double>::sub, lhs, static_cast<long double>(rhs));
            return obj;
        }
        if(std::is_same<secondtype, unknown>::value)
        {
            constexpr const op_tree<firsttype,secondtype>
            obj(OpType<firsttype,secondtype>::sub, lhs, rhs);
            return obj;
        }
        if(is_generic_op_tree<secondtype>::value)
        {
            constexpr const op_tree<firsttype,secondtype>
            obj(OpType<firsttype,secondtype>::sub, lhs, rhs);
            return obj;
        }
    }
}

template<typename firsttype, typename secondtype>
OPER OPER_overload_resolve<firsttype,secondtype>
operator-(const firsttype& lhs, const secondtype& rhs)
{
    if(std::is_same<firsttype, typename is_numtype<firsttype>::type>::value)
    {
        if(std::is_same<secondtype, typename is_numtype<secondtype>::type>::value)
        {
            return static_cast<long double>(lhs-rhs);
        }
        if(std::is_same<secondtype, known>::value)
        {
            return static_cast<long double>(lhs-rhs.value);
        }
        if(std::is_same<secondtype, unknown>::value)
        {
            constexpr const op_tree<long double,secondtype>
            obj(OpType<long double,secondtype>::sub, static_cast<long double>(lhs), rhs);
            return obj;
        }
        if(is_generic_op_tree<secondtype>::value)
        {
            constexpr const op_tree<long double,secondtype>
            obj(OpType<long double,secondtype>::sub, static_cast<long double>(lhs), rhs);
            return obj;
        }
    }
    if(std::is_same<firsttype, known>::value)
    {
        if(std::is_same<secondtype, typename is_numtype<secondtype>::type>::value)
        {
            return static_cast<long double>(lhs.value-rhs);
        }
        if(std::is_same<secondtype, known>::value)
        {
            return static_cast<long double>(lhs.value-rhs.value);
        }
        if(std::is_same<secondtype, unknown>::value)
        {
            constexpr const op_tree<long double,secondtype>
            obj(OpType<long double,secondtype>::sub, static_cast<long double>(lhs.value), rhs);
            return obj;
        }
        if(is_generic_op_tree<secondtype>::value)
        {
            constexpr const op_tree<long double,secondtype>
            obj(OpType<long double,secondtype>::sub, static_cast<long double>(lhs.value), rhs);
            return obj;
        }
    }
    if(std::is_same<firsttype, unknown>::value)
    {
        if(std::is_same<secondtype, typename is_numtype<secondtype>::type>::value)
        {
            constexpr const op_tree<firsttype,long double>
            obj(OpType<firsttype,long double>::sub, lhs, static_cast<long double>(rhs));
            return obj;
        }
        if(std::is_same<secondtype, known>::value)
        {
            constexpr const op_tree<firsttype,long double>
            obj(OpType<firsttype,long double>::sub, lhs, static_cast<long double>(rhs.value));
            return obj;
        }
        if(std::is_same<secondtype, unknown>::value)
        {
            constexpr const op_tree<firsttype,secondtype>
            obj(OpType<firsttype,secondtype>::sub, lhs, rhs);
            return obj;
        }
        if(is_generic_op_tree<secondtype>::value)
        {
            constexpr const op_tree<firsttype,secondtype>
            obj(OpType<firsttype,secondtype>::sub, lhs, rhs);
            return obj;
        }
    }
    if(is_generic_op_tree<firsttype>::value)
    {
        if(std::is_same<secondtype, typename is_numtype<secondtype>::type>::value)
        {
            constexpr const op_tree<firsttype,long double>
            obj(OpType<firsttype,long double>::sub, lhs, static_cast<long double>(rhs));
            return obj;
        }
        if(std::is_same<secondtype, known>::value)
        {
            constexpr const op_tree<firsttype,long double>
            obj(OpType<firsttype,long double>::sub, lhs, static_cast<long double>(rhs.value));
            return obj;
        }
        if(std::is_same<secondtype, unknown>::value)
        {
            constexpr const op_tree<firsttype,secondtype>
            obj(OpType<firsttype,secondtype>::sub, lhs, rhs);
            return obj;
        }
        if(is_generic_op_tree<secondtype>::value)
        {
            constexpr const op_tree<firsttype,secondtype>
            obj(OpType<firsttype,secondtype>::sub, lhs, rhs);
            return obj;
        }
    }
}

template<typename firsttype, typename secondtype>
OPER OPER_overload_resolve<firsttype,secondtype>
operator*(const firsttype& lhs, const secondtype& rhs)
{
    if(std::is_same<firsttype, typename is_numtype<firsttype>::type>::value)
    {
        if(std::is_same<secondtype, typename is_numtype<secondtype>::type>::value)
        {
            return static_cast<long double>(lhs*rhs);
        }
        if(std::is_same<secondtype, known>::value)
        {
            return static_cast<long double>(lhs*rhs.value);
        }
        if(std::is_same<secondtype, unknown>::value)
        {
            constexpr const op_tree<long double,secondtype>
            obj(OpType<long double,secondtype>::mult, static_cast<long double>(lhs), rhs);
            return obj;
        }
        if(is_generic_op_tree<secondtype>::value)
        {
            constexpr const op_tree<long double,secondtype>
            obj(OpType<long double,secondtype>::mult, static_cast<long double>(lhs), rhs);
            return obj;
        }
    }
    if(std::is_same<firsttype, known>::value)
    {
        if(std::is_same<secondtype, typename is_numtype<secondtype>::type>::value)
        {
            return static_cast<long double>(lhs.value*rhs);
        }
        if(std::is_same<secondtype, known>::value)
        {
            return static_cast<long double>(lhs.value*rhs.value);
        }
        if(std::is_same<secondtype, unknown>::value)
        {
            constexpr const op_tree<long double,secondtype>
            obj(OpType<long double,secondtype>::mult, static_cast<long double>(lhs.value), rhs);
            return obj;
        }
        if(is_generic_op_tree<secondtype>::value)
        {
            constexpr const op_tree<long double,secondtype>
            obj(OpType<long double,secondtype>::mult, static_cast<long double>(lhs.value), rhs);
            return obj;
        }
    }
    if(std::is_same<firsttype, unknown>::value)
    {
        if(std::is_same<secondtype, typename is_numtype<secondtype>::type>::value)
        {
            constexpr const op_tree<firsttype, long double>
            obj(OpType<firsttype, long double>::mult, lhs, static_cast<long double>(rhs));
            return obj;
        }
        if(std::is_same<secondtype, known>::value)
        {
            constexpr const op_tree<firsttype, long double>
            obj(OpType<firsttype, long double>::mult, lhs, static_cast<long double>(rhs.value));
            return obj;
        }
        if(std::is_same<secondtype, unknown>::value)
        {
            constexpr const op_tree<firsttype, secondtype>
            obj(OpType<firsttype, secondtype>::mult, lhs, rhs);
            return obj;
        }
        if(is_generic_op_tree<secondtype>::value)
        {
            constexpr const op_tree<firsttype, secondtype>
            obj(OpType<firsttype, secondtype>::mult, lhs, rhs);
            return obj;
        }
    }
    if(is_generic_op_tree<firsttype>::value)
    {
        if(std::is_same<secondtype, typename is_numtype<secondtype>::type>::value)
        {
            constexpr const op_tree<firsttype, long double>
            obj(OpType<firsttype, long double>::mult, lhs, static_cast<long double>(rhs));
            return obj;
        }
        if(std::is_same<secondtype, known>::value)
        {
            constexpr const op_tree<firsttype, long double>
            obj(OpType<firsttype, long double>::mult, lhs, static_cast<long double>(rhs.value));
            return obj;
        }
        if(std::is_same<secondtype, unknown>::value)
        {
            constexpr const op_tree<firsttype, secondtype>
            obj(OpType<firsttype, secondtype>::mult, lhs, rhs);
            return obj;
        }
        if(is_generic_op_tree<secondtype>::value)
        {
            constexpr const op_tree<firsttype, secondtype>
            obj(OpType<firsttype, secondtype>::mult, lhs, rhs);
            return obj;
        }
    }
}

template<typename firsttype, typename secondtype>
OPER OPER_overload_resolve<firsttype,secondtype>
operator/(const firsttype& lhs, const secondtype& rhs)
{
    if(std::is_same<firsttype, typename is_numtype<firsttype>::type>::value)
    {
        if(std::is_same<secondtype, typename is_numtype<secondtype>::type>::value)
        {
            return static_cast<long double>(lhs/rhs);
        }
        if(std::is_same<secondtype, known>::value)
        {
            return static_cast<long double>(lhs/rhs.value);
        }
        if(std::is_same<secondtype, unknown>::value)
        {
            constexpr const op_tree<long double,secondtype>
            obj(OpType<long double,secondtype>::div, static_cast<long double>(lhs), rhs);
            return obj;
        }
        if(is_generic_op_tree<secondtype>::value)
        {
            constexpr const op_tree<long double,secondtype>
            obj(OpType<long double,secondtype>::div, static_cast<long double>(lhs), rhs);
            return obj;
        }
    }
    if(std::is_same<firsttype, known>::value)
    {
        if(std::is_same<secondtype, typename is_numtype<secondtype>::type>::value)
        {
            return static_cast<long double>(lhs.value/rhs);
        }
        if(std::is_same<secondtype, known>::value)
        {
            return static_cast<long double>(lhs.value/rhs.value);
        }
        if(std::is_same<secondtype, unknown>::value)
        {
            constexpr const op_tree<long double,secondtype>
            obj(OpType<long double,secondtype>::div, static_cast<long double>(lhs.value), rhs);
            return obj;
        }
        if(is_generic_op_tree<secondtype>::value)
        {
            constexpr const op_tree<long double,secondtype>
            obj(OpType<long double,secondtype>::div, static_cast<long double>(lhs.value), rhs);
            return obj;
        }
    }
    if(std::is_same<firsttype, unknown>::value)
    {
        if(std::is_same<secondtype, typename is_numtype<secondtype>::type>::value)
        {
            constexpr const op_tree<firsttype,long double>
            obj(OpType<firsttype,long double>::div, lhs, static_cast<long double>(rhs));
            return obj;
        }
        if(std::is_same<secondtype, known>::value)
        {
            constexpr const op_tree<firsttype,long double>
            obj(OpType<firsttype,long double>::div, lhs, static_cast<long double>(rhs.value));
            return obj;
        }
        if(std::is_same<secondtype, unknown>::value)
        {
            constexpr const op_tree<firsttype,secondtype>
            obj(OpType<firsttype,secondtype>::div, lhs, rhs);
            return obj;
        }
        if(is_generic_op_tree<secondtype>::value)
        {
            constexpr const op_tree<firsttype,secondtype>
            obj(OpType<firsttype,secondtype>::div, lhs, rhs);
            return obj;
        }
    }
    if(is_generic_op_tree<firsttype>::value)
    {
        if(std::is_same<secondtype, typename is_numtype<secondtype>::type>::value)
        {
            constexpr const op_tree<firsttype,long double>
            obj(OpType<firsttype,long double>::div, lhs, static_cast<long double>(rhs));
            return obj;
        }
        if(std::is_same<secondtype, known>::value)
        {
            constexpr const op_tree<firsttype,long double>
            obj(OpType<firsttype,long double>::div, lhs, static_cast<long double>(rhs));
            return obj;
        }
        if(std::is_same<secondtype, unknown>::value)
        {
            constexpr const op_tree<firsttype,secondtype>
            obj(OpType<firsttype,secondtype>::div, lhs, rhs);
            return obj;
        }
        if(is_generic_op_tree<secondtype>::value)
        {
            constexpr const op_tree<firsttype,secondtype>
            obj(OpType<firsttype,secondtype>::div, lhs, rhs);
            return obj;
        }
    }
}

    #undef OPER

#endif // VAR_
