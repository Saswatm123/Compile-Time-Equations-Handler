#ifndef VAR_
#define VAR_

#include "op_tree.hpp"
#include "self_ref.hpp"
#include "innertype_calc.hpp"
#include "restriction_calc.hpp"
#include "CT_counter.hpp"
#include <cmath> //NAN

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

#endif // VAR_
