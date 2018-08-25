#ifndef VAR_
#define VAR_

#include "op_tree.hpp"
#include "self_ref.hpp"
#include "innertype_calc.hpp"
#include "restriction_calc.hpp"
#include "CT_counter.hpp"
#include <cmath> //NAN

//Operation impls also #included at bottom

#ifndef OPER
    #define OPER inline constexpr const //OPERator return type modifier
#endif // OPER

#define PERM static constexpr const

template<std::size_t IDinp, bool known>
class var
{
public:
    constexpr var(long double inp = NAN)
    :value(inp)
    {}

    const long double value;

    static constexpr const std::size_t ID = IDinp;

    static constexpr const bool def = known;
};

//inp: K C UK OP
//op_nodes: UK, op_tree<...>, constant

#include "divide_impl.hpp"
#include "mult_impl.hpp"
#include "add_impl.hpp"
#include "subtract_impl.hpp"
#include "equals_impl.hpp"

    #undef OPER

#endif // VAR_
