#ifndef OP_TREE
#define OP_TREE

#include "self_ref.hpp"
#include "EIF_filler.hpp"
#include "innertype_calc.hpp"
#include "restriction_calc.hpp"

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

    template<typename firsttype, typename secondtype>
    using Ins_OpType = typename op_tree<firsttype,secondtype>::OpType;

    static constexpr const OpType InvOp[4] = {sub, add, div, mult};

    const OpType operation;

    const Ltype left;
    const Rtype right;

    constexpr op_tree(OpType opname, const Ltype& left_inp, const Rtype& right_inp)
    :operation(opname), left(left_inp), right(right_inp)
    {}

private:

    //replacing namespace with struct w/ all static members
    struct NI_impl
    {
        template<typename is_op_tree, typename is_known,
                 typename std::enable_if<std::is_arithmetic<is_op_tree>::value, EI_type>::type...
                 >
        static constexpr const auto NI_sub(const is_op_tree& OT, const is_known& new_inf)
        {
            return OT;
        }

        struct NI_sub_tree_impl
        {
            template<typename LHS_, typename RHS_>
            static constexpr const auto NI_sub_tree(int A)
            {
                switch(A)
                {
                    case 0: return Ins_OpType<LHS_, RHS_>::add;
                    case 1: return Ins_OpType<LHS_, RHS_>::sub;
                    case 2: return Ins_OpType<LHS_, RHS_>::mult;
                    case 3: return Ins_OpType<LHS_, RHS_>::div;
                    case 4: return Ins_OpType<LHS_, RHS_>::equal;
                }
            }
        };

        template<typename is_op_tree, typename is_known, typename std::enable_if<is_generic_op_tree<const is_op_tree>::value, EI_type>::type...>
        static const auto NI_sub(const is_op_tree& OT, const is_known& new_inf)
        {
            auto LHS = NI_sub(OT.left , new_inf);
            auto RHS = NI_sub(OT.right, new_inf);

            return op_tree<decltype(LHS), decltype(RHS)>
                          (NI_sub_tree_impl::template NI_sub_tree<decltype(LHS), decltype(RHS)>(static_cast<int>(OT.operation)),
                           LHS, RHS);
        }

        struct NI_sub_UK_impl
        {
            template<typename is_UK, typename is_known,
            typename std::enable_if<is_UK::ID==is_known::ID, EI_type>::type...
                     >
            static const auto NI_sub_UK(const is_UK& UK, const is_known& new_inf)
            {
                return UK.value;
            }

            template<typename is_UK, typename is_known,
            typename std::enable_if<is_UK::ID!=is_known::ID, EI_type>::type...
                     >
            static const auto NI_sub_UK(const is_UK& UK, const is_known& new_inf)
            {
                return UK;
            }
        };

        template<typename is_op_tree, typename is_known,
        typename std::enable_if<is_unknown_failsafe<const is_op_tree>::value, EI_type>::type...>
        inline static const auto NI_sub(const is_op_tree& OT, const is_known& new_inf)
        {
            return NI_sub_UK_impl::NI_sub_UK(OT, new_inf);
        }
    };

public:

    //overload for known's corresponding unknown being in the tree
    template<typename is_op_tree, typename is_known, bool has_ID, typename std::enable_if<has_ID, EI_type>::type...>
    static const auto NI_op_tree(const is_op_tree& OT, const is_known& new_inf)
    {
        //check both sides, clip if no matching unknown, eval if there is unknown
        return NI_impl::NI_sub(OT, new_inf);
    }

    //overload for known's corresponding unknown not being in the tree
    template<typename is_op_tree, typename is_known, bool has_ID, typename std::enable_if<!has_ID, EI_type>::type...>
    static const auto NI_op_tree(const is_op_tree& OT, const is_known& new_inf)
    {
        return OT;
    }
};

template<typename firsttype, typename secondtype>
using OpType = typename op_tree<firsttype,secondtype>::OpType;

#endif // OP_TREE
