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
        //To add new enum val, add to end only, and changes should be
        //reflected in NI_sub_tree_IMPL below

        add,
        sub,
        mult,
        div,
        equal,
        exp,
        compose,
        log
    };

    template<typename firsttype, typename secondtype>
    using Ins_OpType = typename op_tree<firsttype,secondtype>::OpType;

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
        /**
        *   NI_sub = New Information Substitution
        *
        *   Handles finding the unknown in a tree that matches a corresponding newly known variable,
        *   and seeing if the variables are the same (external equation set solved for a variable, and now
        *   we check if that var is in this tree as an unknown.) If so, return value to replace node with.
        */

        //node in node-replacement is number
        template<typename is_op_tree, typename is_known,
                 typename std::enable_if<std::is_arithmetic<const typename is_op_tree::Ctype>::value, EI_type&>::type...
                 >
        static constexpr const auto
        NI_sub(const is_op_tree& OT_C, const is_known& new_inf_C)
        {
            return OT_C.unpack();
        }

        //implementation namespace of logic for function below
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
                    case 5: return Ins_OpType<LHS_, RHS_>::exp;
                    case 6: return Ins_OpType<LHS_, RHS_>::compose;
                    case 7: return Ins_OpType<LHS_, RHS_>::log;
                }
            }
        };

        //node in node-replacement is op_tree
        template<typename is_op_tree, typename is_known,
                 typename std::enable_if<is_generic_op_tree<const typename is_op_tree::Ctype>::value, EI_type&>::type...
                 >
        static constexpr const auto
        NI_sub(const is_op_tree& OT_C, const is_known& new_inf_C)
        {
            constexpr auto leftside  = OT_C.unpack().left;

            constexpr auto rightside = OT_C.unpack().right;

            using CXPR_left  = PACK(leftside);

            using CXPR_right = PACK(rightside);

            constexpr auto LHS = NI_sub(CXPR_left {}, new_inf_C);
            constexpr auto RHS = NI_sub(CXPR_right{}, new_inf_C);

            return op_tree<decltype(LHS), decltype(RHS)>
            (NI_sub_tree_impl::template
             NI_sub_tree<decltype(LHS), decltype(RHS)>(static_cast<int>(OT_C.unpack().operation)), LHS, RHS
             );
        }

        //implementation namespace of logic for function below
        struct NI_sub_UK_impl
        {
            //found UK's ID matches known's ID
            template<typename is_UK, typename is_known,
            typename std::enable_if<is_UK::Ctype::ID==is_known::Ctype::ID, EI_type&>::type...
                     >
            static constexpr const auto
            NI_sub_UK(const is_UK& UK_C, const is_known& new_inf_C)
            {
                return new_inf_C.unpack().value;
            }

            //found UK's ID doesn't match known's ID
            template<typename is_UK, typename is_known,
            typename std::enable_if<is_UK::Ctype::ID!=is_known::Ctype::ID, EI_type&>::type...
                     >
            static constexpr const auto
            NI_sub_UK(const is_UK& UK_C, const is_known& new_inf_C)
            {
                return UK_C.unpack();
            }
        };

        //node in node-replacement is unknown, check if it is the unknown that matches the newly known variable
        template<typename is_op_tree, typename is_known,
        typename std::enable_if<is_unknown_failsafe<const typename is_op_tree::Ctype>::value, EI_type&>::type...>
        inline static constexpr const auto
        NI_sub(const is_op_tree& OT_C, const is_known& new_inf_C)
        {
            return NI_sub_UK_impl::NI_sub_UK(OT_C, new_inf_C);
        }

        //node in node-replacement is unary_ftype
        template<typename is_op_tree, typename is_known,
                 typename std::enable_if<is_unary_ftype<const typename is_op_tree::Ctype>::value, EI_type&>::type...
                 >
        static constexpr const auto
        NI_sub(const is_op_tree& OT_C, const is_known& new_inf_C)
        {
            return OT_C.unpack();
        }
    };

public:

    //overload for known's corresponding unknown being in the tree
    template<bool has_ID, typename is_op_tree, typename is_known, typename std::enable_if<has_ID, EI_type&>::type...>
    static constexpr auto NI_op_tree(const is_op_tree& OT_C, const is_known& new_inf_C)
    {
        static_assert(is_generic_op_tree<const typename is_op_tree::Ctype>::value, "First arg is not OP wrapper"    );
        static_assert(is_known_failsafe <const typename is_known::Ctype  >::value, "Second arg is not Known wrapper");

        //check both sides, clip if no matching unknown, eval if there is unknown
        constexpr auto N = NI_impl::NI_sub(OT_C, new_inf_C);

        typename std::enable_if<is_generic_op_tree<const decltype(N)>::value, int>::type rr = 0;

        return N;
    }

    //overload for known's corresponding unknown not being in the tree
    template<bool has_ID, typename is_op_tree, typename is_known, typename std::enable_if<!has_ID, EI_type&>::type...>
    static constexpr auto NI_op_tree(const is_op_tree& OT_C, const is_known& new_inf_C)
    {
        return OT_C.unpack();
    }
};

template<typename firsttype, typename secondtype>
using OpType = typename op_tree<firsttype,secondtype>::OpType;

#endif // OP_TREE
