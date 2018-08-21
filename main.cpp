#include <iostream>
#include <typeinfo>
#include "var.hpp"

#ifndef PERM
    #define PERM static constexpr const
#endif // PERM

using namespace std;

int main()
{
    constexpr known force(12); constexpr known mass(3); constexpr unknown acceleration;
    constexpr auto ans = force/mass;
}
