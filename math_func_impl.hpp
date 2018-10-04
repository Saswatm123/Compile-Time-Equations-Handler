#ifndef MATH_FUNC_IMPL
#define MATH_FUNC_IMPL

#include "var.hpp"
#include <cmath>
#include "restriction_calc.hpp"

/**
*   sin, cos, tan, arcsin, arccos, arctan,
*   csc, sec, cot, arccsc, arcsec, arccot,
*   log(base, result),
*/

//inptype triangle: unknown, known, op_tree

//Start f(UK)

template<typename is_UK,
         typename std::enable_if<is_unknown<const is_UK>::value, EI_type&>::type...>
OPER op_tree<unary_ftype, is_UK>
sin(const is_UK& UK)
{
    return op_tree<unary_ftype, is_UK>(op_tree<unary_ftype, is_UK>::compose, unary_ftype(0), UK);
}

template<typename is_UK,
         typename std::enable_if<is_unknown<const is_UK>::value, EI_type&>::type...>
OPER op_tree<unary_ftype, is_UK>
cos(const is_UK& UK)
{
    return op_tree<unary_ftype, is_UK>(op_tree<unary_ftype, is_UK>::compose, unary_ftype(1), UK);
}

template<typename is_UK,
         typename std::enable_if<is_unknown<const is_UK>::value, EI_type&>::type...>
OPER op_tree<unary_ftype, is_UK>
tan(const is_UK& UK)
{
    return op_tree<unary_ftype, is_UK>(op_tree<unary_ftype, is_UK>::compose, unary_ftype(2), UK);
}

template<typename is_UK,
         typename std::enable_if<is_unknown<const is_UK>::value, EI_type&>::type...>
OPER op_tree<unary_ftype, is_UK>
csc(const is_UK& UK)
{
    return op_tree<unary_ftype, is_UK>(op_tree<unary_ftype, is_UK>::compose, unary_ftype(3), UK);
}

template<typename is_UK,
         typename std::enable_if<is_unknown<const is_UK>::value, EI_type&>::type...>
OPER op_tree<unary_ftype, is_UK>
sec(const is_UK& UK)
{
    return op_tree<unary_ftype, is_UK>(op_tree<unary_ftype, is_UK>::compose, unary_ftype(4), UK);
}

template<typename is_UK,
         typename std::enable_if<is_unknown<const is_UK>::value, EI_type&>::type...>
OPER op_tree<unary_ftype, is_UK>
cot(const is_UK& UK)
{
    return op_tree<unary_ftype, is_UK>(op_tree<unary_ftype, is_UK>::compose, unary_ftype(5), UK);
}

template<typename is_UK,
         typename std::enable_if<is_unknown<const is_UK>::value, EI_type&>::type...>
OPER op_tree<unary_ftype, is_UK>
arcsin(const is_UK& UK)
{
    return op_tree<unary_ftype, is_UK>(op_tree<unary_ftype, is_UK>::compose, unary_ftype(6), UK);
}

template<typename is_UK,
         typename std::enable_if<is_unknown<const is_UK>::value, EI_type&>::type...>
OPER op_tree<unary_ftype, is_UK>
arccos(const is_UK& UK)
{
    return op_tree<unary_ftype, is_UK>(op_tree<unary_ftype, is_UK>::compose, unary_ftype(7), UK);
}

template<typename is_UK,
         typename std::enable_if<is_unknown<const is_UK>::value, EI_type&>::type...>
OPER op_tree<unary_ftype, is_UK>
arctan(const is_UK& UK)
{
    return op_tree<unary_ftype, is_UK>(op_tree<unary_ftype, is_UK>::compose, unary_ftype(8), UK);
}

template<typename is_UK,
         typename std::enable_if<is_unknown<const is_UK>::value, EI_type&>::type...>
OPER op_tree<unary_ftype, is_UK>
arccsc(const is_UK& UK)
{
    return op_tree<unary_ftype, is_UK>(op_tree<unary_ftype, is_UK>::compose, unary_ftype(9), UK);
}

template<typename is_UK,
         typename std::enable_if<is_unknown<const is_UK>::value, EI_type&>::type...>
OPER op_tree<unary_ftype, is_UK>
arcsec(const is_UK& UK)
{
    return op_tree<unary_ftype, is_UK>(op_tree<unary_ftype, is_UK>::compose, unary_ftype(10), UK);
}

template<typename is_UK,
         typename std::enable_if<is_unknown<const is_UK>::value, EI_type&>::type...>
OPER op_tree<unary_ftype, is_UK>
arccot(const is_UK& UK)
{
    return op_tree<unary_ftype, is_UK>(op_tree<unary_ftype, is_UK>::compose, unary_ftype(11), UK);
}

//end f(UK)
//start f(K)

template<typename is_K,
         typename std::enable_if<is_known<const is_K>::value, EI_type&>::type...>
OPER long double
sin(const is_K& K)
{
    return sin(K.value);
}

template<typename is_K,
         typename std::enable_if<is_known<const is_K>::value, EI_type&>::type...>
OPER long double
cos(const is_K& K)
{
    return cos(K.value);
}

template<typename is_K,
         typename std::enable_if<is_known<const is_K>::value, EI_type&>::type...>
OPER long double
tan(const is_K& K)
{
    return tan(K.value);
}

template<typename is_K,
         typename std::enable_if<is_known<const is_K>::value, EI_type&>::type...>
OPER long double
csc(const is_K& K)
{
    return 1.0/sin(K.value);
}

template<typename is_K,
         typename std::enable_if<is_known<const is_K>::value, EI_type&>::type...>
OPER long double
sec(const is_K& K)
{
    return 1.0/cos(K.value);
}

template<typename is_K,
         typename std::enable_if<is_known<const is_K>::value, EI_type&>::type...>
OPER long double
cot(const is_K& K)
{
    return 1.0/tan(K.value);
}

template<typename is_K,
         typename std::enable_if<is_known<const is_K>::value, EI_type&>::type...>
OPER long double
arcsin(const is_K& K)
{
    return asin(K.value);
}

template<typename is_K,
         typename std::enable_if<is_known<const is_K>::value, EI_type&>::type...>
OPER long double
arccos(const is_K& K)
{
    return acos(K.value);
}

template<typename is_K,
         typename std::enable_if<is_known<const is_K>::value, EI_type&>::type...>
OPER long double
arctan(const is_K& K)
{
    return atan(K.value);
}

template<typename is_K,
         typename std::enable_if<is_known<const is_K>::value, EI_type&>::type...>
OPER long double
arccsc(const is_K& K)
{
    return asin(1.0/(K.value) );
}

template<typename is_K,
         typename std::enable_if<is_known<const is_K>::value, EI_type&>::type...>
OPER long double
arcsec(const is_K& K)
{
    return acos(1.0/K.value);
}

template<typename is_K,
         typename std::enable_if<is_known<const is_K>::value, EI_type&>::type...>
OPER long double
arccot(const is_K& K)
{
    return atan(1.0/K.value);
}

//end f(K)
//start f(OP)

template<typename is_OP,
         typename std::enable_if<is_generic_op_tree<const is_OP>::value, EI_type&>::type...>
OPER op_tree<unary_ftype, is_OP>
sin(const is_OP& OP)
{
    return op_tree<unary_ftype, is_OP>(op_tree<unary_ftype, is_OP>::compose, unary_ftype(0), OP);
}

template<typename is_OP,
         typename std::enable_if<is_generic_op_tree<const is_OP>::value, EI_type&>::type...>
OPER op_tree<unary_ftype, is_OP>
cos(const is_OP& OP)
{
    return op_tree<unary_ftype, is_OP>(op_tree<unary_ftype, is_OP>::compose, unary_ftype(1), OP);
}

template<typename is_OP,
         typename std::enable_if<is_generic_op_tree<const is_OP>::value, EI_type&>::type...>
OPER op_tree<unary_ftype, is_OP>
tan(const is_OP& OP)
{
    return op_tree<unary_ftype, is_OP>(op_tree<unary_ftype, is_OP>::compose, unary_ftype(2), OP);
}

template<typename is_OP,
         typename std::enable_if<is_generic_op_tree<const is_OP>::value, EI_type&>::type...>
OPER op_tree<unary_ftype, is_OP>
csc(const is_OP& OP)
{
    return op_tree<unary_ftype, is_OP>(op_tree<unary_ftype, is_OP>::compose, unary_ftype(3), OP);
}

template<typename is_OP,
         typename std::enable_if<is_generic_op_tree<const is_OP>::value, EI_type&>::type...>
OPER op_tree<unary_ftype, is_OP>
sec(const is_OP& OP)
{
    return op_tree<unary_ftype, is_OP>(op_tree<unary_ftype, is_OP>::compose, unary_ftype(4), OP);
}

template<typename is_OP,
         typename std::enable_if<is_generic_op_tree<const is_OP>::value, EI_type&>::type...>
OPER op_tree<unary_ftype, is_OP>
cot(const is_OP& OP)
{
    return op_tree<unary_ftype, is_OP>(op_tree<unary_ftype, is_OP>::compose, unary_ftype(5), OP);
}

template<typename is_OP,
         typename std::enable_if<is_generic_op_tree<const is_OP>::value, EI_type&>::type...>
OPER op_tree<unary_ftype, is_OP>
arcsin(const is_OP& OP)
{
    return op_tree<unary_ftype, is_OP>(op_tree<unary_ftype, is_OP>::compose, unary_ftype(6), OP);
}

template<typename is_OP,
         typename std::enable_if<is_generic_op_tree<const is_OP>::value, EI_type&>::type...>
OPER op_tree<unary_ftype, is_OP>
arccos(const is_OP& OP)
{
    return op_tree<unary_ftype, is_OP>(op_tree<unary_ftype, is_OP>::compose, unary_ftype(7), OP);
}

template<typename is_OP,
         typename std::enable_if<is_generic_op_tree<const is_OP>::value, EI_type&>::type...>
OPER op_tree<unary_ftype, is_OP>
arctan(const is_OP& OP)
{
    return op_tree<unary_ftype, is_OP>(op_tree<unary_ftype, is_OP>::compose, unary_ftype(8), OP);
}

template<typename is_OP,
         typename std::enable_if<is_generic_op_tree<const is_OP>::value, EI_type&>::type...>
OPER op_tree<unary_ftype, is_OP>
arccsc(const is_OP& OP)
{
    return op_tree<unary_ftype, is_OP>(op_tree<unary_ftype, is_OP>::compose, unary_ftype(9), OP);
}

template<typename is_OP,
         typename std::enable_if<is_generic_op_tree<const is_OP>::value, EI_type&>::type...>
OPER op_tree<unary_ftype, is_OP>
arcsec(const is_OP& OP)
{
    return op_tree<unary_ftype, is_OP>(op_tree<unary_ftype, is_OP>::compose, unary_ftype(10), OP);
}

template<typename is_OP,
         typename std::enable_if<is_generic_op_tree<const is_OP>::value, EI_type&>::type...>
OPER op_tree<unary_ftype, is_OP>
arccot(const is_OP& OP)
{
    return op_tree<unary_ftype, is_OP>(op_tree<unary_ftype, is_OP>::compose, unary_ftype(11), OP);
}

//end f(OP)
//begin f(C)

template<typename is_constant,
         typename std::enable_if<is_arithmetic<const is_constant>::value, EI_type&>::type...>
OPER long double
csc(const is_constant& num)
{
    return 1.0/sin(num);
}

template<typename is_constant,
         typename std::enable_if<is_arithmetic<const is_constant>::value, EI_type&>::type...>
OPER long double
sec(const is_constant& num)
{
    return 1.0/cos(num);
}

template<typename is_constant,
         typename std::enable_if<is_arithmetic<const is_constant>::value, EI_type&>::type...>
OPER long double
cot(const is_constant& num)
{
    return 1.0/tan(num);
}

template<typename is_constant,
         typename std::enable_if<is_arithmetic<const is_constant>::value, EI_type&>::type...>
OPER long double
arcsin(const is_constant& num)
{
    return asin(num);
}

template<typename is_constant,
         typename std::enable_if<is_arithmetic<const is_constant>::value, EI_type&>::type...>
OPER long double
arccos(const is_constant& num)
{
    return acos(num);
}

template<typename is_constant,
         typename std::enable_if<is_arithmetic<const is_constant>::value, EI_type&>::type...>
OPER long double
arctan(const is_constant& num)
{
    return atan(num);
}

template<typename is_constant,
         typename std::enable_if<is_arithmetic<const is_constant>::value, EI_type&>::type...>
OPER long double
arccsc(const is_constant& num)
{
    return asin(1.0/num);
}

template<typename is_constant,
         typename std::enable_if<is_arithmetic<const is_constant>::value, EI_type&>::type...>
OPER long double
arcsec(const is_constant& num)
{
    return acos(1.0/num);
}

template<typename is_constant,
         typename std::enable_if<is_arithmetic<const is_constant>::value, EI_type&>::type...>
OPER long double
arccot(const is_constant& num)
{
    return atan(1.0/num);
}

//end f(C)

#endif // MATH_FUNC_IMPL
