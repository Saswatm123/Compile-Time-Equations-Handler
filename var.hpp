#ifndef VAR_
#define VAR_

#include "op_tree.hpp"
#include "self_ref.hpp"
#include "innertype_calc.hpp"
#include <cmath> //NAN

//Operation impls also #included at bottom

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

//inp both operation types, checks if second is op_tree
template<typename first, typename second>
using R_mod_overload_resolve = typename std::enable_if<is_generic_op_tree<second>::value,op_tree<first, second> >::type;

//inp both operation types, checks if first is op_tree
template<typename first, typename second>
using L_mod_overload_resolve = typename std::enable_if<is_generic_op_tree<first>::value,op_tree<first, second> >::type;

//enables op_tree<A,B> only if A and B are both op_tree objs
template<typename A, typename B>
using both_mod_overload_resolve =
typename std::enable_if<
is_generic_op_tree<A>::value && is_generic_op_tree<B>::value, op_tree<A,B>
                        >::type;

//inp: K C UK OP
//op_nodes: UK, op_tree<...>, constant

#include "divide_impl.hpp"
#include "mult_impl.hpp"
#include "add_impl.hpp"
#include "subtract_impl.hpp"
//Future add "equal_impl.hpp"

    #undef OPER

#endif // VAR_
