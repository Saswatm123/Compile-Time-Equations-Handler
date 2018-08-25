#ifndef EQUALS_IMPL
#define EQUALS_IMPL

#include "var.hpp"
#include "restriction_calc.hpp"

//inp: K C UK OP
//op vals: bool K OP

template<typename known_check_LHS, typename known_check_RHS>
OPER
bool_if_both_known<const known_check_LHS, const known_check_RHS>
operator==(const known_check_LHS& lhs, const known_check_RHS& rhs)
{
    return lhs.value==rhs.value;
}

template<typename known_check_LHS>
OPER
typename std::enable_if<is_known<const known_check_LHS>::value, bool>::type
operator==(const known_check_LHS& lhs, const long double& rhs)
{
    return lhs.value==rhs;
}

template<typename known_check_LHS, typename unknown_check_RHS>
OPER
typename std::enable_if<is_known<const known_check_LHS>::value && is_unknown<const unknown_check_RHS>::value,
                        op_tree<known_check_LHS, unknown_check_RHS>
                        >::type
operator==(const known_check_LHS& lhs, const unknown_check_RHS& rhs)
{
    return op_tree<known_check_LHS, unknown_check_RHS>
    (OpType<known_check_LHS, unknown_check_RHS>::equal, lhs, rhs);
}

template<typename known_check_LHS, typename OP_check_RHS>
OPER
typename std::enable_if<is_known<const known_check_LHS>::value && is_generic_op_tree<OP_check_RHS>::value,
                        op_tree<known_check_LHS, OP_check_RHS>
                        >::type
operator==(const known_check_LHS& lhs, const OP_check_RHS& rhs)
{
    return op_tree<known_check_LHS, OP_check_RHS>
    (OpType<known_check_LHS, OP_check_RHS>::equal, lhs, rhs);
}

template<typename known_check_RHS>
OPER
typename std::enable_if<is_known<const known_check_RHS>::value, bool>
operator==(const long double& lhs, const known_check_RHS& rhs)
{
    return lhs==rhs.value;
}

template<typename unknown_check_RHS>
OPER
K_if_UK<const unknown_check_RHS>
operator==(const long double& lhs, const unknown_check_RHS& rhs)
{
    return var<unknown_check_RHS::ID ,1>(lhs);
}

template<typename OP_check_RHS>
OPER
typename std::enable_if<is_generic_op_tree<OP_check_RHS>::value,
                        op_tree<long double, OP_check_RHS>
                        >::type
operator==(const long double& lhs, const OP_check_RHS& rhs)
{
    return op_tree<long double, OP_check_RHS>
    (OpType<long double, OP_check_RHS>::equal, lhs, rhs);
}

template<typename unknown_check_LHS, typename known_check_RHS>
OPER
typename std::enable_if<is_unknown<const unknown_check_LHS>::value && is_known<known_check_RHS>::value,
                        var<unknown_check_LHS::ID,1>
                        >::type
operator==(const unknown_check_LHS& lhs, const known_check_RHS& rhs)
{
    return var<unknown_check_LHS::ID,1>(lhs);
}

template<typename unknown_check_LHS>
OPER
K_if_UK<const unknown_check_LHS>
operator==(const unknown_check_LHS& lhs, const long double& rhs)
{
    return var<unknown_check_LHS::ID, 1>(rhs);
}

template<typename unknown_check_LHS, typename unknown_check_RHS>
OPER
typename std::enable_if<is_unknown<const unknown_check_LHS>::value && is_unknown<const unknown_check_RHS>::value,
                        op_tree<unknown_check_LHS, unknown_check_RHS>
                        >::type
operator==(const unknown_check_LHS& lhs, const unknown_check_RHS& rhs)
{
    return op_tree<unknown_check_LHS, unknown_check_RHS>
    (OpType<unknown_check_LHS, unknown_check_RHS>::equal, lhs, rhs);
}

template<typename unknown_check_LHS, typename OP_check_RHS>
OPER
typename std::enable_if<is_unknown<const unknown_check_LHS>::value && is_generic_op_tree<OP_check_RHS>::value,
                        op_tree<unknown_check_LHS, OP_check_RHS>
                        >::type
operator==(const unknown_check_LHS& lhs, const OP_check_RHS& rhs)
{
    return op_tree<unknown_check_LHS, OP_check_RHS>
    (OpType<unknown_check_LHS, OP_check_RHS>::equal, lhs, rhs);
}

template<typename OP_check_LHS, typename known_check_RHS>
OPER
typename std::enable_if<is_generic_op_tree<OP_check_LHS>::value && is_known<const known_check_RHS>::value,
                        op_tree<OP_check_LHS, long double>
                        >::type
operator==(const OP_check_LHS& lhs, const known_check_RHS& rhs)
{
    return op_tree<OP_check_LHS, long double>
    (OpType<OP_check_LHS, known_check_RHS>::equal, lhs, static_cast<long double>(rhs.value));
}

template<typename OP_check_LHS>
OPER
typename std::enable_if<is_generic_op_tree<OP_check_LHS>::value, op_tree<OP_check_LHS, long double> >::type
operator==(const OP_check_LHS& lhs, const long double& rhs)
{
    return op_tree<OP_check_LHS, long double>
    (lhs, rhs);
}

template<typename OP_check_LHS, typename unknown_check_RHS>
OPER
typename std::enable_if<is_generic_op_tree<OP_check_LHS>::value && is_unknown<const unknown_check_RHS>::value,
                        op_tree<OP_check_LHS, unknown_check_RHS>
                        >::type
operator==(const OP_check_LHS& lhs, const unknown_check_RHS& rhs)
{
    return op_tree<OP_check_LHS, unknown_check_RHS>
    (OpType<OP_check_LHS, unknown_check_RHS>::equal, lhs, rhs);
}

template<typename OP_check_LHS, typename OP_check_RHS>
OPER
typename std::enable_if<is_generic_op_tree<OP_check_LHS>::value && is_generic_op_tree<OP_check_RHS>::value,
                        op_tree<OP_check_LHS, OP_check_RHS>
                        >::type
operator==(const OP_check_LHS& lhs, const OP_check_RHS& rhs)
{
    return op_tree<OP_check_LHS, OP_check_RHS>
    (OpType<OP_check_LHS, OP_check_RHS>::equal, lhs, rhs);
}

#endif // EQUALS_IMPL
