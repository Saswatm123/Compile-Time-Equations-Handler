#ifndef DIVIDE_IMPL
#define DIVIDE_IMPL

#include "var.hpp"

//inp: K C UK OP
//op_nodes: UK, op_tree<...>, constant

OPER long double operator/(const known& lhs, const known& rhs)
{
    return static_cast<long double>(lhs.value/rhs.value);
}

OPER long double operator/(const known& lhs, const long double& rhs)
{
    return static_cast<long double>(lhs.value/rhs);
}

OPER op_tree<long double, unknown> operator/(const known& lhs, const unknown& rhs)
{
    return op_tree<long double, unknown>(OpType<long double, unknown>::div, lhs.value, rhs);
}

template<typename is_op_tree>
OPER R_mod_overload_resolve<long double,is_op_tree> operator/(const known& lhs, const is_op_tree& rhs)
{
    return op_tree<long double, is_op_tree>(OpType<long double, is_op_tree>::div, static_cast<long double>(lhs.value), rhs);
}

OPER long double operator/(const long double& lhs, const known& rhs)
{
    return lhs/rhs.value;
}

OPER op_tree<long double, unknown> operator/(const long double& lhs, const unknown& rhs)
{
    return op_tree<long double, unknown>(OpType<long double, unknown>::div, lhs, rhs);
}

template<typename is_op_tree>
OPER R_mod_overload_resolve<long double,is_op_tree> operator/(const long double& lhs, const is_op_tree& rhs)
{
    return op_tree<long double, is_op_tree>(OpType<long double, is_op_tree>::div, lhs, rhs);
}

OPER op_tree<unknown, long double> operator/(const unknown& lhs, const known& rhs)
{
    return op_tree<unknown, long double>(OpType<unknown, long double>::div, lhs, static_cast<long double>(rhs.value));
}

OPER op_tree<unknown, long double> operator/(const unknown& lhs, const long double& rhs)
{
    return op_tree<unknown, long double>(OpType<unknown, long double>::div, lhs, rhs);
}

OPER op_tree<unknown, unknown> operator/(const unknown& lhs, const unknown& rhs)
{
    return op_tree<unknown, unknown>(OpType<unknown, unknown>::div, lhs, rhs);
}

template<typename is_op_tree>
OPER R_mod_overload_resolve<unknown, is_op_tree> operator/(const unknown& lhs, const is_op_tree& rhs)
{
    return op_tree<unknown, is_op_tree>(OpType<unknown, is_op_tree>::div, lhs, rhs);
}

template<typename is_op_tree>
OPER L_mod_overload_resolve<is_op_tree, long double> operator/(const is_op_tree& lhs, const known& rhs)
{
    return op_tree<is_op_tree, long double>(OpType<is_op_tree, long double>::div, lhs, static_cast<long double>(rhs.value));
}

template<typename is_op_tree>
OPER L_mod_overload_resolve<is_op_tree, long double> operator/(const is_op_tree& lhs, const long double& rhs)
{
    return op_tree<is_op_tree, long double>(OpType<is_op_tree, long double>::div, lhs, rhs);
}

template<typename is_op_tree>
OPER L_mod_overload_resolve<is_op_tree, unknown> operator/(const is_op_tree& lhs, const unknown& rhs)
{
    return op_tree<is_op_tree, unknown>(OpType<is_op_tree, unknown>::div, lhs, rhs);
}

template<typename A, typename B>
OPER both_mod_overload_resolve<A,B> operator/(const A& lhs, const B& rhs)
{
    return op_tree<A,B>(OpType<A,B>::div, lhs, rhs);
}

#endif // DIVIDE_IMPL
