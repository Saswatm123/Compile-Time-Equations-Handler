#ifndef EQUATION_SHEET
#define EQUATION_SHEET

namespace equation_sheet
{
    inline namespace
    PUT_VARIABLES_HERE
    {
        known force(12);
        known mass(3);
        known distance(5);
        unknown acceleration;
        unknown time;
    }

    inline namespace
    PUT_TARGET_VARIABLE_HERE
    {
        /**
        *   Name target variable TARGVAR.
        *   Format is:
        *   using TARGVAR = PACK( (insert target variable here) );
        */

        using TARGVAR = PACK(time);
    }

    inline namespace
    PUT_EQUATIONS_HERE
    {
        /**
        *   When using exponentials, always put it in parentheses, so a^2 -> (a^2)
        *   Always write to one digit after decimal, so 1/2 -> 1.0/2.0
        *   Format is:
        *   using (equation name) = PACK( (insert equation here) );
        */

        using eq1 = PACK(force == mass * acceleration);

        using eq2 = PACK(distance == (1.0/2.0) * acceleration * (time^2));
    }
}

#endif // EQUATION_SHEET
