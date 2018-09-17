#define BITS_Q_STATES 9u ///Inp bits required for storing max amount of variables

#define PACK(...)                            \
    union                                    \
    {                                        \
        static constexpr auto unpack()       \
        {                                    \
            return __VA_ARGS__;              \
        }                                    \
                                             \
        typedef decltype(__VA_ARGS__) Ctype; \
    }

#include <iostream>
#include <iomanip>
#include "equation_processor.hpp"

using namespace std;

/**
*   BASE SAFE FUNCS ARE GET, UK_COUNT, HAS_NOTED_ID, NI_SUB_TREE, SHORT_EVAL, TERNARY,
*   RESOLVE(COMPLIMENTARY), M_TREE_DECIDER
*/

int main()
{
    using namespace equation_sheet;

    constexpr auto ans = iter_through_equations(
    TARGVAR{},
    knownlist{},
    eq1{},
    eq2{}
    );

    cout << setprecision(10) << endl << ans.value;
}
