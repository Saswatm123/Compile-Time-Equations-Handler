#ifndef DIVIDE_IMPL
#define DIVIDE_IMPL

#include "var.hpp"
#include "restriction_calc.hpp"

//inp: K C UK OP
//op_nodes: UK, op_tree<...>, constant

//known, known
template<typename known_check_A, typename known_check_B>
OPER
both_known_ret_type<const known_check_A, const known_check_B>
LOG(const known_check_A& lhs, const known_check_B& rhs)
{
    return static_cast<long double>(log(rhs.value)/log(lhs.value) );
}

//known, constant
template<typename known_check_LHS>
OPER
K_C_ret_type<const known_check_LHS>
LOG(const known_check_LHS& lhs, const long double& rhs)
{
    return static_cast<long double>(log(rhs)/log(lhs.value) );
}

//known, unknown
template<typename known_check_LHS, typename unknown_check_RHS>
OPER
op_tree<LD_if_known<const known_check_LHS>,
        typename std::remove_const<UK_if_UK<const unknown_check_RHS> >::type
        >
LOG(const known_check_LHS& lhs, const unknown_check_RHS& rhs)
{
    return op_tree<long double, unknown_check_RHS>(OpType<long double, unknown_check_RHS>::log, static_cast<long double>(lhs.value), rhs);
}

//known, optree
template<typename known_check_LHS, typename is_op_tree>
OPER
R_mod_overload_resolve<LD_if_known<const known_check_LHS>, is_op_tree>
LOG(const known_check_LHS& lhs, const is_op_tree& rhs)
{
    return op_tree<long double, is_op_tree>(OpType<long double, is_op_tree>::log, static_cast<long double>(lhs.value), rhs);
}

//constant, known
template<typename known_check_RHS>
OPER
LD_if_known<const known_check_RHS>
LOG(const long double& lhs, const known_check_RHS& rhs)
{
    return log(rhs.value)/log(lhs);
}

//constant, constant
OPER long double
LOG(const long double& lhs, const long double& rhs)
{
    return log(rhs)/log(lhs);
}

//constant, unknown
template<typename unknown_check_RHS>
OPER
op_tree<long double,
        typename std::remove_const<UK_if_UK<const unknown_check_RHS> >::type
        >
LOG(const long double& lhs, const unknown_check_RHS& rhs)
{
    return op_tree<long double, unknown_check_RHS>(OpType<long double, unknown_check_RHS>::log, lhs, rhs);
}

//constant, optree
template<typename is_op_tree>
OPER R_mod_overload_resolve<long double,is_op_tree>
LOG(const long double& lhs, const is_op_tree& rhs)
{
    return op_tree<long double, is_op_tree>(OpType<long double, is_op_tree>::log, lhs, rhs);
}

//unknown, known
template<typename unknown_check_LHS, typename known_check_RHS>
OPER
op_tree<typename std::remove_const<UK_if_UK<const unknown_check_LHS> >::type,
        LD_if_known<const known_check_RHS>
        >
LOG(const unknown_check_LHS& lhs, const known_check_RHS& rhs)
{
    return op_tree<unknown_check_LHS, long double>(OpType<unknown_check_LHS, long double>::log, lhs, static_cast<long double>(rhs.value));
}

//unknown, constant
template<typename unknown_check_LHS>
OPER
op_tree<typename std::remove_const<UK_if_UK<unknown_check_LHS> >::type,
        long double
        >
LOG(const unknown_check_LHS& lhs, const long double& rhs)
{
    return op_tree<unknown_check_LHS, long double>(OpType<unknown_check_LHS, long double>::log, lhs, rhs);
}

//unknown, unknown
template<typename unknown_check_LHS, typename unknown_check_RHS>
OPER
op_tree<typename std::remove_const<UK_if_UK<const unknown_check_LHS> >::type,
        typename std::remove_const<UK_if_UK<const unknown_check_RHS> >::type
        >
LOG(const unknown_check_LHS& lhs, const unknown_check_RHS& rhs)
{
    return op_tree<unknown_check_LHS, unknown_check_RHS>(OpType<unknown_check_LHS, unknown_check_RHS>::log, lhs, rhs);
}

//unknown, optree
template<typename unknown_check_LHS, typename is_op_tree>
OPER
R_mod_overload_resolve<typename std::remove_const<UK_if_UK<const unknown_check_LHS> >::type,
                       is_op_tree
                       >
LOG(const unknown_check_LHS& lhs, const is_op_tree& rhs)
{
    return op_tree<unknown_check_LHS, is_op_tree>(OpType<unknown_check_LHS, is_op_tree>::log, lhs, rhs);
}

//optree, known
template<typename is_op_tree, typename known_check_RHS>
OPER
L_mod_overload_resolve<is_op_tree,
                       LD_if_known<const known_check_RHS>
                       >
LOG(const is_op_tree& lhs, const known_check_RHS& rhs)
{
    return op_tree<is_op_tree, long double>(OpType<is_op_tree, long double>::log, lhs, static_cast<long double>(rhs.value));
}

//optree, constant
template<typename is_op_tree>
OPER
L_mod_overload_resolve<is_op_tree, long double>
LOG(const is_op_tree& lhs, const long double& rhs)
{
    return op_tree<is_op_tree, long double>(OpType<is_op_tree, long double>::log, lhs, rhs);
}

//optree, unknown
template<typename is_op_tree, typename unknown_check_RHS>
OPER
L_mod_overload_resolve<is_op_tree,
                       typename std::remove_const<UK_if_UK<const unknown_check_RHS> >::type
                       >
LOG(const is_op_tree& lhs, const unknown_check_RHS& rhs)
{
    return op_tree<is_op_tree, unknown_check_RHS>(OpType<is_op_tree, unknown_check_RHS>::log, lhs, rhs);
}

//optree, optree
template<typename A, typename B>
OPER both_mod_overload_resolve<A,B>
LOG(const A& lhs, const B& rhs)
{
    return op_tree<A,B>(OpType<A,B>::log, lhs, rhs);
}

#endif // DIVIDE_IMPL
