#ifndef OP_TREE
#define OP_TREE

#include "self_ref.hpp"

template<typename lefttype, typename righttype>
class op_tree : public self_ref<op_tree<lefttype,righttype> >
{
public:

    typedef lefttype Ltype;
    typedef righttype Rtype;

    enum OpType //add new operators later (var exp, var log, etc)
    {
        add,
        sub,
        mult,
        div,
        equal
    };

    const OpType operation;
    const Ltype& left;
    const Rtype& right;

    constexpr op_tree(OpType opname, const Ltype& left_inp, const Rtype& right_inp)
    :operation(opname), left(left_inp), right(right_inp)
    {}
};

template<typename firsttype, typename secondtype>
using OpType = typename op_tree<firsttype,secondtype>::OpType;

#endif // OP_TREE
