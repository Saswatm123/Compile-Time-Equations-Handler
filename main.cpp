#define BITS_Q_STATES 9u //Inp bits required for storing max amount of variables

#include <iostream>
#include <typeinfo>
#include "var.hpp"

#include "cmd_list.hpp"
#include "ntuple.hpp"

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

    constexpr auto N = (stuff==acceleration);

    constexpr auto TARGETVAR = mass;

    constexpr const ntuple<decltype(mass)> knownlist(mass);

    constexpr auto f = iter_through_equations(TARGETVAR, knownlist, mass, 'a', 2, 'b');

    cout << endl << f;

    cout << endl << new_information(N, 2);
}
