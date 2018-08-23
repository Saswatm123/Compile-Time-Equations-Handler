#ifndef EQUALS_IMPL
#define EQUALS_IMPL

#include "var.hpp"
#include "restriction_calc.hpp"

//inp: K C UK OP
//op_nodes:

template<typename unknown_check_LHS>
OPER K_if_UK<unknown_check_LHS> operator==(const unknown_check_LHS& lhs, const long double& rhs)
{
    return var<unknown_check_LHS::ID, 1>(rhs);
}

template<typename unknown_check_RHS>
OPER K_if_UK<unknown_check_RHS> operator==(const long double& lhs, const unknown_check_RHS& rhs)
{
    return (rhs==lhs);
}

/*
OPER op_tree<> operator==()
{
    return op_tree<>();
}*/

#endif // EQUALS_IMPL
