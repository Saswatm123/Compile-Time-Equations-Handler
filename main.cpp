#define BITS_Q_STATES 9u //Inp bits required for storing max amount of variables

#define CONSTANT(...)                       \
    union                                   \
    {                                       \
        static constexpr auto value()       \
        {                                   \
            return __VA_ARGS__;             \
        }                                   \
                                            \
        typedef decltype(__VA_ARGS__) type; \
    }

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

template<typename is_op_tree, typename M_op_type, int M_tree, bool side1, bool side2,
                 typename std::enable_if<M_tree == OpType<
                                                          typename M_op_type::Ltype,
                                                          typename M_op_type::Rtype
                                                          >::add, EI_type
                                         >::type...
                 >
constexpr const auto testfunc(const is_op_tree& OT)
{
    return 1;
}

int main()
{
    constexpr op_tree<long double, long double> C(OpType<long double, long double>::add, 2, 3);
    constexpr int a = testfunc<decltype(C), decltype(C), C.operation, 0, 0>(C);

    PERM known force(12);
    PERM known mass(3);
    PERM unknown acceleration;
    PERM unknown stuff;

    static constexpr const op_tree<op_tree<decltype(acceleration), long double>, long double>
    first(OpType<op_tree<decltype(acceleration), long double>, long double>::equal,
                 op_tree<decltype(acceleration), long double>(OpType<decltype(acceleration), long double>::mult,
                                                              acceleration,
                                                              1),
          8);

    /*
    constexpr auto aa = extract_detail::resolve_impl::resolve(first);

    cout << aa;

    constexpr const op_tree<var<2,0>, op_tree<long double, long double> > finalnamed(OpType<var<2,0>, op_tree<long double, long double> >::equal,acceleration,
                                                                                     op_tree<long double, long double>(OpType<long double, long double>::add, 1, 2) );*/


    using CXPR_first = CONSTANT(first);
    constexpr auto solved = extract_detail::extract_known_main(CXPR_first{});

    cout << endl << solved.value;
/*
    cout << endl << TESTfunc(finalnamed);*/

/*
    cout << endl << TESTfunc<decltype(finalnamed)>(finalnamed);*/
}
