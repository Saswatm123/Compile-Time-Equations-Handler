#ifndef UNARY_FUNC_TYPE
#define UNARY_FUNC_TYPE

/**
*   Created from sin/cos/tan, their reversals, their inversions,
*   partial deriv
*/

class unary_ftype
{
public:

    static const bool _is_unary_ftype = 1;

    enum operation_list
    {
           sin,    cos,    tan,    csc,    sec,    cot,
        arcsin, arccos, arctan, arccsc, arcsec, arccot,
        P_DERIV
    };

    const operation_list indicator;

    constexpr unary_ftype(const int N)
    :indicator(static_cast<operation_list>(N))
    {}
};

#endif // UNARY_FUNC_TYPE
