#ifndef EQUATION_SHEET
#define EQUATION_SHEET

/**
*   First, define all relevant variables as known or unknown.
*   Next, in any order, list the target variable by writing
*       TARGET( (variable name) )
*   and list the equations by writing
*       EQUATION( (equation name), (equation content) )
*   once for every equation.
*
*   - When using exponentials, always use parentheses, so a^2 -> (a^2)
*   - Always write to one digit after decimal, so 1/2 -> 1.0/2.0
*/

known   force(12);
known   mass(3);
known   distance(5);
unknown acceleration;
unknown time;

TARGET(time)

EQUATION(eq1, force == mass * acceleration)

EQUATION(eq2, distance == (1.0/2.0) * acceleration * (time^2))

#endif // EQUATION_SHEET
