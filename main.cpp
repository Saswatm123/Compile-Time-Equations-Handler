#define BITS_Q_STATES 9u //Inp bits required for storing max amount of variables

#include <iostream>
#include <typeinfo>
#include "var.hpp"

#ifndef PERM
    #define PERM static constexpr const
#endif // PERM

using namespace std;

int main()
{
    PERM known   force       (12,ticket());
    PERM known   mass        (3 ,ticket());
    PERM unknown acceleration(   ticket());
    PERM unknown stuff       (   ticket());

    cout << force.ID << " " << mass.ID << " " << acceleration.ID << " " << stuff.ID;
}
