#ifndef SUBTRACT_IMPL
#define SUBTRACT_IMPL

#include "var.hpp"
#include "restriction_calc.hpp"

//inp: K C UK OP
//op_nodes: UK, op_tree<...>, constant

template<typename known_check_A, typename known_check_B>
<<<<<<< HEAD
OPER both_known_ret_type<const known_check_A, const known_check_B> operator-(const known_check_A& lhs, const known_check_B& rhs)
=======
OPER both_known_ret_type<known_check_A, known_check_B> operator-(const known_check_A& lhs, const known_check_B& rhs)
>>>>>>> bc925241b99bcaaffb4b89d31ddab6ccd60a0a4f
{
    return static_cast<long double>(lhs.value-rhs.value);
}

template<typename known_check_LHS>
<<<<<<< HEAD
OPER
K_C_ret_type<const known_check_LHS>
operator-(const known_check_LHS& lhs, const long double& rhs)
=======
OPER K_C_ret_type<known_check_LHS> operator-(const known_check_LHS& lhs, const long double& rhs)
>>>>>>> bc925241b99bcaaffb4b89d31ddab6ccd60a0a4f
{
    return static_cast<long double>(lhs.value-rhs);
}

<<<<<<< HEAD
template<typename known_check_LHS, typename unknown_check_RHS>
OPER op_tree<LD_if_known<const known_check_LHS>,
             typename std::remove_const<UK_if_UK<const unknown_check_RHS> >::type
             >
operator-(const known_check_LHS& lhs, const unknown_check_RHS& rhs)
=======
template<typename known_check_LHS,typename unknown_check_RHS>
OPER op_tree<LD_if_known<known_check_LHS>, UK_if_UK<unknown_check_RHS> > operator-(const known_check_LHS& lhs, const unknown_check_RHS& rhs)
>>>>>>> bc925241b99bcaaffb4b89d31ddab6ccd60a0a4f
{
    return op_tree<long double, unknown_check_RHS>(OpType<long double, unknown_check_RHS>::sub, static_cast<long double>(lhs.value), rhs);
}

template<typename known_check_LHS, typename is_op_tree>
<<<<<<< HEAD
OPER
R_mod_overload_resolve<LD_if_known<const known_check_LHS>, is_op_tree>
operator-(const known_check_LHS& lhs, const is_op_tree& rhs)
=======
OPER R_mod_overload_resolve<LD_if_known<known_check_LHS>, is_op_tree> operator-(const known_check_LHS& lhs, const is_op_tree& rhs)
>>>>>>> bc925241b99bcaaffb4b89d31ddab6ccd60a0a4f
{
    return op_tree<long double, is_op_tree>(OpType<long double, is_op_tree>::sub, static_cast<long double>(lhs.value), rhs);
}

template<typename known_check_RHS>
<<<<<<< HEAD
OPER
LD_if_known<const known_check_RHS>
operator-(const long double& lhs, const known_check_RHS& rhs)
=======
OPER LD_if_known<known_check_RHS> operator-(const long double& lhs, const known_check_RHS& rhs)
>>>>>>> bc925241b99bcaaffb4b89d31ddab6ccd60a0a4f
{
    return lhs-rhs.value;
}

template<typename unknown_check_RHS>
<<<<<<< HEAD
OPER
op_tree<long double,
        typename std::remove_const<UK_if_UK<const unknown_check_RHS> >::type
        >
operator-(const long double& lhs, const unknown_check_RHS& rhs)
=======
OPER op_tree<long double, UK_if_UK<unknown_check_RHS> > operator-(const long double& lhs, const unknown_check_RHS& rhs)
>>>>>>> bc925241b99bcaaffb4b89d31ddab6ccd60a0a4f
{
    return op_tree<long double, unknown_check_RHS>(OpType<long double, unknown_check_RHS>::sub, lhs, rhs);
}

template<typename is_op_tree>
OPER
R_mod_overload_resolve<long double,is_op_tree>
operator-(const long double& lhs, const is_op_tree& rhs)
{
    return op_tree<long double, is_op_tree>(OpType<long double, is_op_tree>::sub, lhs, rhs);
}

template<typename unknown_check_LHS, typename known_check_RHS>
<<<<<<< HEAD
OPER
op_tree<typename std::remove_const<UK_if_UK<const unknown_check_LHS> >::type,
        LD_if_known<const known_check_RHS>
        >
operator-(const unknown_check_LHS& lhs, const known_check_RHS& rhs)
=======
OPER op_tree<UK_if_UK<unknown_check_LHS>, LD_if_known<known_check_RHS> > operator-(const unknown_check_LHS& lhs, const known_check_RHS& rhs)
>>>>>>> bc925241b99bcaaffb4b89d31ddab6ccd60a0a4f
{
    return op_tree<unknown_check_LHS, long double>(OpType<unknown_check_LHS, long double>::sub, lhs, static_cast<long double>(rhs.value));
}

template<typename unknown_check_LHS>
<<<<<<< HEAD
OPER
op_tree<typename std::remove_const<UK_if_UK<const unknown_check_LHS> >::type,
        long double>
operator-(const unknown_check_LHS& lhs, const long double& rhs)
=======
OPER op_tree<UK_if_UK<unknown_check_LHS>, long double> operator-(const unknown_check_LHS& lhs, const long double& rhs)
>>>>>>> bc925241b99bcaaffb4b89d31ddab6ccd60a0a4f
{
    return op_tree<unknown_check_LHS, long double>(OpType<unknown_check_LHS, long double>::sub, lhs, rhs);
}

template<typename unknown_check_LHS, typename unknown_check_RHS>
<<<<<<< HEAD
OPER
op_tree<typename std::remove_const<UK_if_UK<const unknown_check_LHS> >::type,
        typename std::remove_const<UK_if_UK<const unknown_check_RHS> >::type
        >
operator-(const unknown_check_LHS& lhs, const unknown_check_RHS& rhs)
=======
OPER op_tree<UK_if_UK<unknown_check_LHS>, UK_if_UK<unknown_check_RHS> > operator-(const unknown_check_LHS& lhs, const unknown_check_RHS& rhs)
>>>>>>> bc925241b99bcaaffb4b89d31ddab6ccd60a0a4f
{
    return op_tree<unknown_check_LHS, unknown_check_RHS>(OpType<unknown_check_LHS, unknown_check_RHS>::sub, lhs, rhs);
}

template<typename unknown_check_LHS, typename is_op_tree>
<<<<<<< HEAD
OPER
R_mod_overload_resolve<typename std::remove_const<UK_if_UK<const unknown_check_LHS> >::type,
                       is_op_tree
                       >
operator-(const unknown_check_LHS& lhs, const is_op_tree& rhs)
=======
OPER R_mod_overload_resolve<UK_if_UK<unknown_check_LHS>, is_op_tree> operator-(const unknown_check_LHS& lhs, const is_op_tree& rhs)
>>>>>>> bc925241b99bcaaffb4b89d31ddab6ccd60a0a4f
{
    return op_tree<unknown_check_LHS, is_op_tree>(OpType<unknown_check_LHS, is_op_tree>::sub, lhs, rhs);
}

template<typename is_op_tree, typename known_check_RHS>
<<<<<<< HEAD
OPER
L_mod_overload_resolve<is_op_tree,
                       LD_if_known<const known_check_RHS>
                       >
operator-(const is_op_tree& lhs, const known_check_RHS& rhs)
=======
OPER L_mod_overload_resolve<is_op_tree, LD_if_known<known_check_RHS> > operator-(const is_op_tree& lhs, const known_check_RHS& rhs)
>>>>>>> bc925241b99bcaaffb4b89d31ddab6ccd60a0a4f
{
    return op_tree<is_op_tree, long double>(OpType<is_op_tree, long double>::sub, lhs, static_cast<long double>(rhs.value));
}

template<typename is_op_tree>
OPER
L_mod_overload_resolve<is_op_tree, long double>
operator-(const is_op_tree& lhs, const long double& rhs)
{
    return op_tree<is_op_tree, long double>(OpType<is_op_tree, long double>::sub, lhs, rhs);
}

template<typename is_op_tree, typename unknown_check_RHS>
<<<<<<< HEAD
OPER
L_mod_overload_resolve<is_op_tree,
                       typename std::remove_const<UK_if_UK<const unknown_check_RHS> >::type
                       >
operator-(const is_op_tree& lhs, const unknown_check_RHS& rhs)
=======
OPER L_mod_overload_resolve<is_op_tree, UK_if_UK<unknown_check_RHS> > operator-(const is_op_tree& lhs, const unknown_check_RHS& rhs)
>>>>>>> bc925241b99bcaaffb4b89d31ddab6ccd60a0a4f
{
    return op_tree<is_op_tree, unknown_check_RHS>(OpType<is_op_tree, unknown_check_RHS>::sub, lhs, rhs);
}

template<typename A, typename B>
OPER both_mod_overload_resolve<A,B> operator-(const A& lhs, const B& rhs)
{
    return op_tree<A,B>(OpType<A,B>::sub, lhs, rhs);
}

#endif // SUBTRACT_IMPL
