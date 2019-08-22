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
*/

unknown X;
unknown Y;
unknown Z;
unknown A;
unknown B;
known C(3);

TARGET(X)

EQUATION(eq1, arcsec(X)-Y-Z == 1)
EQUATION(eq2, (C^B) == 9)
EQUATION(eq3, Y + Z + A == 4)
EQUATION(eq4, (Y^B) == 2+2)
EQUATION(eq5, LOG(A, 64) + Y == 5)

#endif // EQUATION_SHEET
