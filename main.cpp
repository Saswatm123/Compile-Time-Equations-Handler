#define BITS_Q_STATES 9u ///Inp bits required for storing max amount of variables

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

    constexpr auto ans = SOLVE(
	eq1{},
	eq2{},
	eq3{},
	eq4{},
	eq5{}
	);

    cout << setprecision(10) << ans.value;
}
