<<<<<<< HEAD
#define BITS_Q_STATES 9u //Inp bits required for storing max amount of variables

#include <iostream>
#include <typeinfo>
#include "var.hpp"

#ifndef PERM
    #define PERM static constexpr const
#endif // PERM

#define known   var<ticket(),1>
#define unknown var<ticket(),0>

using namespace std;

int main()
{
    PERM known force(12);
    PERM known mass(3);
    PERM unknown acceleration;
    PERM unknown stuff;
}
=======
#define BITS_Q_STATES 9u //Inp bits required for storing max amount of variables

#include <iostream>
#include <typeinfo>
#include "var.hpp"

#ifndef PERM
    #define PERM static constexpr const
#endif // PERM

#define known   var<ticket(),1>
#define unknown var<ticket(),0>

using namespace std;

int main()
{
    PERM known force(12);
    PERM known mass(3);
    PERM unknown acceleration;
    PERM unknown stuff;
}
>>>>>>> 6b4acc599ff09c04d9bbcac5b5a808dc19dc6208
