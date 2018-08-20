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
        equality
    };

    const OpType operation;

    constexpr op_tree(OpType opname)
    :operation(opname)
    {}
};

template<typename firsttype, typename secondtype>
using OpType = typename op_tree<firsttype,secondtype>::OpType;

#endif // OP_TREE
