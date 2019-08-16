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

int main()
{
    using namespace equation_sheet;

    constexpr auto ans = SOLVE(
    eq1{},
    eq2{}
    );

    cout << setprecision(10) << endl << ans.value;
}
