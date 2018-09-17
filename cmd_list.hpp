#ifndef CMD_LIST
#define CMD_LIST

#include "void_t.hpp"
#include "innertype_calc.hpp"
#include "ntuple.hpp"
#include "EIF_filler.hpp"
#include "OP_numeric.hpp"

//BEGIN FWD DECLARATIONS FOR ITER_THROUGH_EQUATIONS


    template<typename TVAR_TYPE, typename ntuple_type, typename firstarg, typename... arglist,
             typename std::enable_if<std::is_same<bool, typename firstarg::Ctype>::value, EI_type&>::type...>
    constexpr auto iter_through_equations(const TVAR_TYPE&, const ntuple_type&,
                                          const firstarg& , const arglist&...);

    template<typename TVAR_TYPE, typename ntuple_type, typename firstarg, typename... arglist,
             typename std::enable_if<is_generic_op_tree<const typename firstarg::Ctype>::value &&
             (UK_count_TS<typename firstarg::Ctype>() == 1), EI_type&>::type...>
    constexpr auto iter_through_equations(const TVAR_TYPE&, const ntuple_type&,
                                          const firstarg& , const arglist&...);

    template<typename TVAR_TYPE, typename ntuple_type, typename firstarg, typename... arglist,
             typename std::enable_if<is_generic_op_tree<const typename firstarg::Ctype>::value &&
             (UK_count_TS<typename firstarg::Ctype>() > 1) &&
             ntuple_type::Ctype::get_size() != 0, EI_type&>::type...>
    constexpr auto iter_through_equations(const TVAR_TYPE&, const ntuple_type&,
                                          const firstarg& , const arglist&...);

    template<typename TVAR_TYPE, typename ntuple_type, typename firstarg, typename... arglist,
             typename std::enable_if<is_generic_op_tree<const typename firstarg::Ctype>::value &&
             (UK_count_TS<typename firstarg::Ctype>() > 1) &&
             ntuple_type::Ctype::get_size() == 0, EI_type&>::type...>
    constexpr auto iter_through_equations(const TVAR_TYPE&, const ntuple_type&,
                                          const firstarg& , const arglist&...);

    template<typename TVAR_TYPE, typename ntuple_type, typename firstarg, typename... arglist,
             typename std::enable_if<is_known<const typename firstarg::Ctype>::value &&
             TVAR_TYPE::Ctype::ID == firstarg::Ctype::ID, EI_type&>::type...>
    constexpr auto iter_through_equations(const TVAR_TYPE&, const ntuple_type&,
                                          const firstarg& , const arglist&...);

    template<typename TVAR_TYPE, typename ntuple_type, typename firstarg, typename... arglist,
             typename std::enable_if<is_known<const typename firstarg::Ctype>::value &&
             TVAR_TYPE::Ctype::ID != firstarg::Ctype::ID, EI_type&>::type...>
    constexpr auto iter_through_equations(const TVAR_TYPE&, const ntuple_type&,
                                          const firstarg& , const arglist&...);

//END FWD DECLARATIONS FOR ITER_THROUGH_EQUATIONS

template<typename is_op_tree, typename newinf_t,
typename std::enable_if<is_generic_op_tree<const typename is_op_tree::Ctype>::value &&
                        is_known_failsafe<const typename newinf_t::Ctype>::value, EI_type&>::type...>
inline constexpr const auto
new_information(const is_op_tree& basetree_C, const newinf_t& newinf_C)
{
    //NI it and UKcount check afterwards. if UKcount afterwards
    //results in 1, eval numerically. Return type splitter

    static_assert(is_generic_op_tree<const decltype(basetree_C.unpack() )>::value, "First arg doesnt wrap an op_tree");
    static_assert(is_known_failsafe <const typename newinf_t::Ctype      >::value, "Second arg doesn't wrap a known" );

    constexpr auto N = is_op_tree::Ctype::template NI_op_tree<has_noted_ID(basetree_C.unpack(), newinf_C.unpack().ID)>(basetree_C, newinf_C);

    return N;

}

//Contains tree inversion algos, known extraction, resolution + eval
namespace extract_detail
{
    template<bool A, typename first, typename second, typename std::enable_if<A, EI_type>::type...>
    constexpr const auto
    ternary(const first& firstobj, const second& secondobj)
    {
        return firstobj;
    }

    template<bool A, typename first, typename second, typename std::enable_if<!A, EI_type>::type...>
    constexpr const auto
    ternary(const first& firstobj, const second& secondobj)
    {
        return secondobj;
    }

    template<typename is_UK, typename std::enable_if<is_unknown<const is_UK>::value, EI_type>::type...>
    inline constexpr const var<is_UK::ID, 1>
    short_eval(int operation, const is_UK& lhs, const long double rhs)
    {
        using OP = op_tree<char,char>;

        if(operation==OP::equal)
        {
            return lhs==rhs;
        }
    }

    template<typename is_UK, typename std::enable_if<is_unknown<const is_UK>::value, EI_type>::type...>
    inline constexpr const var<is_UK::ID, 1>
    short_eval(int operation, const long double lhs, const is_UK& rhs)
    {
        using OP = op_tree<char,char>;

        if(operation==OP::equal)
        {
            return lhs==rhs;
        }
    }

    //SE should assume operation is not equals, have handler filter out
    inline constexpr const long double
    short_eval(int operation, const long double lhs, const long double rhs)
    {
        using OP = op_tree<char,char>;

        if(operation == OP::add)
        {
            return lhs+rhs;
        }
        if(operation == OP::div)
        {
            return lhs/rhs;
        }
        if(operation == OP::sub)
        {
            return lhs-rhs;
        }
        if(operation == OP::mult)
        {
            return lhs*rhs;
        }
        if(operation == OP::exp)
        {
            return std::pow(lhs,rhs);
        }
    }

    namespace resolve_impl
    {
        /**
        *   Take in op_tree of all ops and constants and
        *   simplify to one constant. Variants are C-C (base case),
        *   C-O, O-C, and O-O. Called from EKM in order to simplify
        *   side branch
        */

        //Begin fwd declarations

            template<typename is_op_tree, typename std::enable_if<std::is_arithmetic<typename is_op_tree::Rtype>::value &&
                                                   !std::is_arithmetic<typename is_op_tree::Ltype>::value, EI_type&>::type...>
            inline constexpr const long double
            resolve(const is_op_tree&);

            template<typename is_op_tree, typename std::enable_if<std::is_arithmetic<typename is_op_tree::Ltype>::value &&
                                                   !std::is_arithmetic<typename is_op_tree::Rtype>::value, EI_type&>::type...>
            inline constexpr const long double
            resolve(const is_op_tree&);

            template<typename is_op_tree,
                     typename std::enable_if<!std::is_arithmetic<typename is_op_tree::Ltype>::value &&
                                             !std::is_arithmetic<typename is_op_tree::Rtype>::value,
                                             EI_type&
                                             >::type...
                     >
            inline constexpr const long double
            resolve(const is_op_tree&);

            template<typename is_op_tree, typename std::enable_if<std::is_arithmetic<typename is_op_tree::Ltype>::value &&
                                                                  std::is_arithmetic<typename is_op_tree::Rtype>::value,
                                                                  EI_type&
                                                                  >::type...
                     >
            inline constexpr const long double
            resolve(const is_op_tree&);

            inline constexpr const long double
            resolve(const long double);

        //End fwd declaration, begin definitions

        inline constexpr const long double
        resolve(const long double val)
        {
            return val;
        }

        template<typename is_op_tree, typename std::enable_if<std::is_arithmetic<typename is_op_tree::Ltype>::value &&
                                                              std::is_arithmetic<typename is_op_tree::Rtype>::value,
                                                              EI_type&
                                                              >::type...
                 >
        inline constexpr const long double
        resolve(const is_op_tree& OT)
        {
            return short_eval(OT.operation, OT.left, OT.right);
        }

        template<typename is_op_tree, typename std::enable_if<std::is_arithmetic<typename is_op_tree::Rtype>::value &&
                                               !std::is_arithmetic<typename is_op_tree::Ltype>::value, EI_type&>::type...>
        inline constexpr const long double
        resolve(const is_op_tree& OT)
        {
            return short_eval(OT.operation, resolve(OT.left), OT.right);
        }

        template<typename is_op_tree, typename std::enable_if<std::is_arithmetic<typename is_op_tree::Ltype>::value &&
                                               !std::is_arithmetic<typename is_op_tree::Rtype>::value, EI_type&>::type...>
        inline constexpr const long double
        resolve(const is_op_tree& OT)
        {
            return short_eval(OT.operation, OT.left, resolve(OT.right) );
        }

        template<typename is_op_tree,
                 typename std::enable_if<!std::is_arithmetic<typename is_op_tree::Ltype>::value &&
                                         !std::is_arithmetic<typename is_op_tree::Rtype>::value,
                                         EI_type&
                                         >::type...
                 >
        inline constexpr const long double
        resolve(const is_op_tree& OT)
        {
            return short_eval(OT.operation, resolve(OT.left), resolve(OT.right));
        }

    }

    namespace extract_impl
    {
        /**
        *   Take in op tree and turn f(UK) = f(C) into UK = f(C).
        *   Different protocol for different operations, sub and div
        *   are side sensitive. Only called from EKM(bottom).
        */

        ///add
        template<int M_tree, bool side1, bool side2, typename is_op_tree,
                 typename std::enable_if<M_tree == OpType<char,char>::add, EI_type&
                                         >::type...
                 >
        inline constexpr const auto
        extract_known(const is_op_tree& OT)
        {
            //extract side
            auto M_before_split = ternary<side1, decltype(OT.right), decltype(OT.left)>(OT.right, OT.left);

            //non-important side
            auto S_branch = ternary<!side1, decltype(OT.right), decltype(OT.left)>(OT.right, OT.left);

            auto helper_left  = M_before_split.right;
            auto helper_right = M_before_split.left ;

            //clip to opp. side
            auto M_branch =
            ternary<!side2,decltype(helper_left),decltype(helper_right)>(helper_left, helper_right);

            auto S_tree = op_tree<decltype(S_branch),decltype(M_branch)>
            (OpType<decltype(S_branch),decltype(M_branch)>::sub, S_branch, M_branch);

            auto T_choice1 = M_before_split.right;
            auto T_choice2 = M_before_split.left ;

            auto new_M_tree = ternary<side2, decltype(T_choice1), decltype(T_choice2)>(T_choice1, T_choice2);

            auto ret_left_type  = ternary<!side1, decltype(new_M_tree), decltype(S_tree)>(new_M_tree, S_tree);
            auto ret_right_type = ternary< side1, decltype(new_M_tree), decltype(S_tree)>(new_M_tree, S_tree);

            return op_tree<decltype(ret_left_type), decltype(ret_right_type)>
            (OpType<decltype(ret_left_type), decltype(ret_right_type)>::equal, ret_left_type, ret_right_type);
        }

        ///sub, MLeft
        template<int M_tree, bool side1, bool side2, typename is_op_tree,
                 typename std::enable_if<M_tree == OpType<char,char>::sub && !side2, EI_type&>::type...
                 >
        inline constexpr const auto
        extract_known(const is_op_tree& OT)
        {
            auto S_branch = ternary<!side1, decltype(OT.right), decltype(OT.left)>(OT.right, OT.left); //non-important side

            auto helper_left  = ternary<side1, decltype(OT.right), decltype(OT.left)>(OT.right, OT.left).right;
            auto helper_right = ternary<side1, decltype(OT.right), decltype(OT.left)>(OT.right, OT.left).left ;

            //clip to opp. side
            auto M_branch =
            ternary<!side2,decltype(helper_left),decltype(helper_right)>(helper_left, helper_right);

            //if side2==1, clip(sub,S,M) -> clip(mult, prev, -1)
            //if side2==0, clip(add,S,M)

            auto S_tree = op_tree<decltype(S_branch), decltype(M_branch)>
            (OpType<decltype(S_branch), decltype(M_branch)>::add, S_branch, M_branch);

            auto helper_1 = ternary<side1, decltype(OT.right), decltype(OT.left)>(OT.right, OT.left).right;
            auto helper_2 = ternary<side1, decltype(OT.right), decltype(OT.left)>(OT.right, OT.left).left;

            auto new_M_tree = ternary<side2, decltype(helper_1), decltype(helper_2)>(helper_1, helper_2);

            auto ret_help_left  = ternary<!side1, decltype(new_M_tree), decltype(S_tree)>(new_M_tree, S_tree);
            auto ret_help_right = ternary< side1, decltype(new_M_tree), decltype(S_tree)>(new_M_tree, S_tree);

            return op_tree<decltype(ret_help_left), decltype(ret_help_right)>
                          (OpType<decltype(ret_help_left), decltype(ret_help_right)>::equal,
                           ret_help_left, ret_help_right);
        }

        ///sub, MRight
        template<int M_tree, bool side1, bool side2, typename is_op_tree,
                 typename std::enable_if<M_tree == OpType<char,char>::sub && side2, EI_type&
                                         >::type...
                 >
        inline constexpr const auto
        extract_known(const is_op_tree& OT)
        {
            auto S_branch = ternary<!side1, decltype(OT.right), decltype(OT.left)>(OT.right, OT.left); //non-important side

            auto helper_left  = ternary<side1, decltype(OT.right), decltype(OT.left)>(OT.right, OT.left).right;
            auto helper_right = ternary<side1, decltype(OT.right), decltype(OT.left)>(OT.right, OT.left).left ;

            //clip to opp. side
            auto M_branch =
            ternary<!side2,decltype(helper_left),decltype(helper_right)>(helper_left, helper_right);

            //if side2==1, clip(sub,S,M) -> clip(mult, prev, -1)

            auto mid_RHS = op_tree<decltype(S_branch),decltype(M_branch)>
                                  (OpType<decltype(S_branch),decltype(M_branch)>::sub, S_branch, M_branch);

            auto final_S_tree = op_tree<decltype(mid_RHS), long double>
                                (OpType<decltype(mid_RHS), long double>::mult, mid_RHS, -1);

            auto helper_1 = ternary<side1, decltype(OT.right), decltype(OT.left)>(OT.right, OT.left).right;
            auto helper_2 = ternary<side1, decltype(OT.right), decltype(OT.left)>(OT.right, OT.left).left ;

            auto new_M_tree = ternary<side2, decltype(helper_1), decltype(helper_2)>(helper_1, helper_2);

            auto ret_help_left  = ternary<!side1, decltype(new_M_tree), decltype(final_S_tree)>(new_M_tree, final_S_tree);
            auto ret_help_right = ternary< side1, decltype(new_M_tree), decltype(final_S_tree)>(new_M_tree, final_S_tree);

            return op_tree<decltype(ret_help_left), decltype(ret_help_right)>
                          (OpType<decltype(ret_help_left), decltype(ret_help_right)>::equal,
                           ret_help_left, ret_help_right);
        }

        ///mult
        template<int M_tree, bool side1, bool side2, typename is_op_tree,
                 typename std::enable_if<M_tree == OpType<char,char>::mult, EI_type&
                                         >::type...
                 >
        inline constexpr const auto
        extract_known(const is_op_tree& OT)
        {
            //non-important side
            auto S_branch = ternary<!side1, decltype(OT.right), decltype(OT.left)>(OT.right, OT.left);

            auto helper_left  = ternary<side1, decltype(OT.right), decltype(OT.left)>(OT.right, OT.left).right;
            auto helper_right = ternary<side1, decltype(OT.right), decltype(OT.left)>(OT.right, OT.left).left ;

            //clip to opp. side
            auto M_branch = ternary<!side2, decltype(helper_left), decltype(helper_right)>(helper_left, helper_right);

            auto S_tree = op_tree<decltype(S_branch),decltype(M_branch)>
            (OpType<decltype(S_branch),decltype(M_branch)>::div, S_branch, M_branch);

            auto helper_1 = ternary<side1, decltype(OT.right), decltype(OT.left)>(OT.right, OT.left).right;
            auto helper_2 = ternary<side1, decltype(OT.right), decltype(OT.left)>(OT.right, OT.left).left ;

            auto new_M_tree = ternary<side2, decltype(helper_1), decltype(helper_2)>(helper_1, helper_2);

            auto ret_help_left  = ternary<!side1, decltype(new_M_tree), decltype(S_tree)>(new_M_tree, S_tree);
            auto ret_help_right = ternary< side1, decltype(new_M_tree), decltype(S_tree)>(new_M_tree, S_tree);

            return op_tree<decltype(ret_help_left), decltype(ret_help_right)>
                          (OpType<decltype(ret_help_left), decltype(ret_help_right)>::equal,
                           ret_help_left, ret_help_right);
        }

        ///div, Mleft
        template<int M_tree, bool side1, bool side2, typename is_op_tree,
                 typename std::enable_if<M_tree == OpType<char,char>::div && !side2, EI_type&
                                         >::type...
                 >
        inline constexpr const auto
        extract_known(const is_op_tree& OT)
        {
            //non-important side
            auto S_branch = ternary<!side1, decltype(OT.right), decltype(OT.left)>(OT.right, OT.left);

            auto helper_left  = ternary<side1, decltype(OT.right), decltype(OT.left)>(OT.right, OT.left).right;
            auto helper_right = ternary<side1, decltype(OT.right), decltype(OT.left)>(OT.right, OT.left).left ;

            //clip to opp. side
            auto M_branch = ternary<!side2, decltype(helper_left), decltype(helper_right)>(helper_left, helper_right);

            auto S_tree = op_tree<decltype(S_branch), decltype(M_branch)>
            (OpType<decltype(S_branch), decltype(M_branch)>::mult, S_branch, M_branch);

            auto helper_1 = ternary<side1, decltype(OT.right), decltype(OT.left)>(OT.right, OT.left).right;
            auto helper_2 = ternary<side1, decltype(OT.right), decltype(OT.left)>(OT.right, OT.left).left ;

            auto new_M_tree = ternary<side2, decltype(helper_1), decltype(helper_2)>(helper_1, helper_2);

            auto ret_help_left  = ternary<!side1, decltype(new_M_tree), decltype(S_tree)>(new_M_tree, S_tree);
            auto ret_help_right = ternary< side1, decltype(new_M_tree), decltype(S_tree)>(new_M_tree, S_tree);

            return op_tree<decltype(ret_help_left), decltype(ret_help_right)>
                          (OpType<decltype(ret_help_left), decltype(ret_help_right)>::equal,
                           ret_help_left, ret_help_right);
        }

        ///div, MRight
        template<int M_tree, bool side1, bool side2, typename is_op_tree,
                 typename std::enable_if<M_tree == OpType<char,char>::div && side2, EI_type&>::type...
                 >
        inline constexpr const auto
        extract_known(const is_op_tree& OT)
        {
            //non-important side
            auto S_branch = ternary<!side1, decltype(OT.right), decltype(OT.left)>(OT.right, OT.left);

            auto M_branch_unsplit = ternary<side1, decltype(OT.right), decltype(OT.left)>(OT.right, OT.left);

            auto helper_left  = M_branch_unsplit.right;
            auto helper_right = M_branch_unsplit.left ;

            //clip to opp. side
            auto M_branch = ternary<!side2, decltype(helper_left), decltype(helper_right)>(helper_left, helper_right);

            auto mid_RHS = op_tree<decltype(S_branch),decltype(M_branch)>
                                  (OpType<decltype(S_branch),decltype(M_branch)>::div, S_branch, M_branch);

            auto final_S_tree = op_tree<long double, decltype(mid_RHS)>
                                (OpType<long double, decltype(mid_RHS)>::div, 1, mid_RHS);

            auto helper_1 = ternary<side1, decltype(OT.right), decltype(OT.left)>(OT.right, OT.left).right;
            auto helper_2 = ternary<side1, decltype(OT.right), decltype(OT.left)>(OT.right, OT.left).left ;

            auto new_M_tree = ternary<side2, decltype(helper_1), decltype(helper_2)>(helper_1, helper_2);

            auto ret_help_left  = ternary<!side1, decltype(new_M_tree), decltype(final_S_tree)>(new_M_tree, final_S_tree);
            auto ret_help_right = ternary< side1, decltype(new_M_tree), decltype(final_S_tree)>(new_M_tree, final_S_tree);

            return op_tree<decltype(ret_help_left), decltype(ret_help_right)>
                          (OpType<decltype(ret_help_left), decltype(ret_help_right)>::equal,
                           ret_help_left, ret_help_right);
        }

        ///exp, Mleft
        template<int M_tree, bool side1, bool side2, typename is_op_tree,
                 typename std::enable_if<M_tree == OpType<char,char>::exp && !side2, EI_type&
                                         >::type...
                 >
        inline constexpr const auto
        extract_known(const is_op_tree& OT)
        {
            //non-important side
            auto S_branch = ternary<!side1, decltype(OT.right), decltype(OT.left)>(OT.right, OT.left);

            auto helper_left  = ternary<side1, decltype(OT.right), decltype(OT.left)>(OT.right, OT.left).right;
            auto helper_right = ternary<side1, decltype(OT.right), decltype(OT.left)>(OT.right, OT.left).left ;

            //clip to opp. side
            auto M_branch = ternary<!side2, decltype(helper_left), decltype(helper_right)>(helper_left, helper_right);

            auto inverted_exp = op_tree<long double, decltype(M_branch)>
            (OpType<long double, decltype(M_branch)>::div, 1, M_branch);

            auto S_tree = op_tree<decltype(S_branch), decltype(inverted_exp)>
            (OpType<decltype(S_branch), decltype(inverted_exp)>::exp, S_branch, inverted_exp);

            auto helper_1 = ternary<side1, decltype(OT.right), decltype(OT.left)>(OT.right, OT.left).right;
            auto helper_2 = ternary<side1, decltype(OT.right), decltype(OT.left)>(OT.right, OT.left).left ;

            auto new_M_tree = ternary<side2, decltype(helper_1), decltype(helper_2)>(helper_1, helper_2);

            auto ret_help_left  = ternary<!side1, decltype(new_M_tree), decltype(S_tree)>(new_M_tree, S_tree);
            auto ret_help_right = ternary< side1, decltype(new_M_tree), decltype(S_tree)>(new_M_tree, S_tree);

            return op_tree<decltype(ret_help_left), decltype(ret_help_right)>
                          (OpType<decltype(ret_help_left), decltype(ret_help_right)>::equal,
                           ret_help_left, ret_help_right);
        }
    }

    namespace EKM_impl
    {
        //only called from EKM, returns tree to remove
        template<bool B, typename first, typename second, typename std::enable_if<B, EI_type&>::type...>
        inline constexpr const auto
        M_tree_decider(const first& first_tree, const second& second_tree)
        {
            return first_tree;
        }

        template<bool B, typename first, typename second, typename std::enable_if<!B, EI_type&>::type...>
        inline constexpr const auto
        M_tree_decider(const first& first_tree, const second& second_tree)
        {
            return second_tree;
        }
    }

    //UK = f(C)
    template<typename is_op_tree, typename std::enable_if<is_unknown_failsafe<const typename is_op_tree::Ctype::Ltype>::value &&
                                                          !is_unknown_failsafe<const typename is_op_tree::Ctype::Rtype>::value,
                                                          EI_type&
                                                          >::type...
             >
    inline constexpr const var<is_op_tree::Ctype::Ltype::ID, 1>
    extract_known_main(const is_op_tree& OT)
    {
        //non UK side
        auto N = EKM_impl::M_tree_decider<is_unknown_failsafe<const typename is_op_tree::Ctype::Ltype>::value,
                                          decltype(OT.unpack().right), decltype(OT.unpack().left)
                                          >(OT.unpack().right, OT.unpack().left);

        auto C_from_func_C = resolve_impl::resolve(N);

        return short_eval(OT.unpack().operation, OT.unpack().left, C_from_func_C);
    }

    //f(C) = UK
    template<typename is_op_tree, typename std::enable_if<!is_unknown_failsafe<const typename is_op_tree::Ctype::Ltype>::value &&
                                                          is_unknown_failsafe<const typename is_op_tree::Ctype::Rtype>::value,
                                                          EI_type&
                                                          >::type...
             >
    inline constexpr const var<is_op_tree::Ctype::Rtype::ID, 1>
    extract_known_main(const is_op_tree& OT)
    {
        //non UK side
        constexpr auto N = EKM_impl::M_tree_decider<is_unknown_failsafe<const typename is_op_tree::Ctype::Ltype>::value>
        (OT.unpack().right, OT.unpack().left);

        constexpr auto C_from_func_C = resolve_impl::resolve(N);

        return short_eval(OT.unpack().operation, C_from_func_C, OT.unpack().right);
    }

    //assumes only 1 UK whole tree
    template<typename is_op_tree, typename std::enable_if<!is_unknown_failsafe<const typename is_op_tree::Ctype::Ltype>::value &&
                                                          !is_unknown_failsafe<const typename is_op_tree::Ctype::Rtype>::value,
                                                          EI_type&
                                                          >::type...
             >
    inline constexpr const auto
    extract_known_main(const is_op_tree& OT_C)
    {
        constexpr std::size_t side = UK_count(OT_C.unpack().right); //0 left 1 right

        constexpr auto make_not_prvalue = ternary<side>(OT_C.unpack().right, OT_C.unpack().left).right;

        constexpr std::size_t d_side = UK_count(make_not_prvalue);

        //node to remove
        constexpr auto M_tree = EKM_impl::M_tree_decider<side>
                      (OT_C.unpack().right, OT_C.unpack().left);

        //tree with most upcoming operand inverted
        constexpr auto passdown = extract_impl::extract_known<M_tree.operation, side, d_side>(OT_C.unpack());

        using CXPR_passdown = PACK(passdown);

        return extract_known_main(CXPR_passdown{}); //Explicitly handles all "equals" cases, no passdown has equals
    }
}

//Plug In Knowns
namespace PIK
{
    /**
    *   Called from ITE. Takes in Equation and list of knowns. Plugs in each known and operates
    *   on updated tree. If UK_count ever == 1, it returns extracted known. If index == last, plugs
    *   in last and returns whatever the result is.
    */

    using namespace extract_detail;

    template<std::size_t UK_num, std::size_t maxind, std::size_t ind, typename is_op_tree, typename ntup_type,
             typename std::enable_if<UK_num==1 && ind==(maxind-1), EI_type&>::type...>
    inline constexpr const auto
    plug_in_knowns(const is_op_tree& OT_C, const ntup_type& knownlist_C)
    {
        return extract_known_main(OT_C);
    }

    template<std::size_t UK_num, std::size_t maxind, std::size_t ind, typename is_op_tree, typename ntup_type,
             typename std::enable_if<UK_num==1 && ind!=(maxind-1), EI_type&>::type...>
    inline constexpr const auto
    plug_in_knowns(const is_op_tree& OT_C, const ntup_type& knownlist_C)
    {
        return extract_known_main(OT_C);
    }

    template<std::size_t UK_num, std::size_t maxind, std::size_t ind, typename is_op_tree, typename ntup_type,
             typename std::enable_if<UK_num!=1 && ind==(maxind-1), EI_type&>::type...>
    inline constexpr const auto
    plug_in_knowns(const is_op_tree& OT_C, const ntup_type& knownlist_C)
    {
        constexpr auto N = get<ind>(knownlist_C.unpack() );

        using CXPR_N = PACK(N);

        return new_information(OT_C, CXPR_N{});
    }

    template<std::size_t UK_num, std::size_t maxind, std::size_t ind, typename is_op_tree, typename ntup_type,
             typename std::enable_if<UK_num!=1 && ind!=(maxind-1) && is_generic_op_tree<const typename is_op_tree::Ctype>::value, EI_type&>::type...>
    inline constexpr const auto
    plug_in_knowns(const is_op_tree& OT_C, const ntup_type& knownlist_C)
    {
        constexpr auto nextobj = get<ind>(knownlist_C.unpack() );

        using CXPR_nextobj = PACK(nextobj);

        constexpr auto Q = new_information(OT_C, CXPR_nextobj{});

        using CXPR_Q = PACK(Q);

        return plug_in_knowns<UK_count(Q), maxind, ind+1>
        (CXPR_Q{}, knownlist_C);
    }
}

//firstarg is bool
template<typename TVAR_TYPE, typename ntuple_type, typename firstarg, typename... arglist,
         typename std::enable_if<std::is_same<bool, typename firstarg::Ctype>::value, EI_type&>::type...>
constexpr auto iter_through_equations(const TVAR_TYPE& TARGETVAR_C, const ntuple_type& knownlist_C,
                                      const firstarg& current_eq_C, const arglist&... pack_C)
{
    static_assert(current_eq_C.unpack(), "An inequality has occurred, fatal error. You ended up with a \"1==0\" type scenario");

    return iter_through_equations(TARGETVAR_C, knownlist_C, pack_C...);
}

//firstarg is op_tree, UK_count == 1
template<typename TVAR_TYPE, typename ntuple_type, typename firstarg, typename... arglist,
         typename std::enable_if<is_generic_op_tree<const typename firstarg::Ctype>::value &&
         (UK_count_TS<typename firstarg::Ctype>() == 1), EI_type&>::type...>
constexpr auto iter_through_equations(const TVAR_TYPE& TARGETVAR_C, const ntuple_type& knownlist_C,
                                      const firstarg& current_eq_C, const arglist&... pack_C)
{
    constexpr auto new_known = extract_detail::extract_known_main(current_eq_C);

    using CXPR_new_known = PACK(new_known);

    return iter_through_equations(TARGETVAR_C, knownlist_C, CXPR_new_known{}, pack_C...);
}

//firstarg is op_tree, UK_count > 1, knownlist size != 0
template<typename TVAR_TYPE, typename ntuple_type, typename firstarg, typename... arglist,
         typename std::enable_if<is_generic_op_tree<const typename firstarg::Ctype>::value &&
         (UK_count_TS<typename firstarg::Ctype>() > 1) &&
         ntuple_type::Ctype::get_size() != 0, EI_type&>::type...>
constexpr auto iter_through_equations(const TVAR_TYPE& TARGETVAR_C, const ntuple_type& knownlist_C,
                                      const firstarg& current_eq_C, const arglist&... pack_C)
{
    constexpr auto N = PIK::plug_in_knowns<UK_count(current_eq_C.unpack() ), knownlist_C.unpack().get_size(), 0>
    (current_eq_C, knownlist_C);

    using CXPR_N = PACK(N);

    //if N is known, check if targvar and return. if N is op_tree, continue as normal
    return iter_through_equations(TARGETVAR_C, knownlist_C, pack_C..., CXPR_N{});
}

//firstarg is op_tree, UK_count > 1, knownlist size == 0
template<typename TVAR_TYPE, typename ntuple_type, typename firstarg, typename... arglist,
         typename std::enable_if<is_generic_op_tree<const typename firstarg::Ctype>::value &&
         (UK_count_TS<typename firstarg::Ctype>() > 1) &&
         ntuple_type::Ctype::get_size() == 0, EI_type&>::type...>
constexpr auto iter_through_equations(const TVAR_TYPE& TARGETVAR_C, const ntuple_type& knownlist_C,
                                      const firstarg& current_eq_C, const arglist&... pack_C)
{
    return iter_through_equations(TARGETVAR_C, knownlist_C, pack_C..., current_eq_C);
}

//firstarg is known, matches TVAR's ID
template<typename TVAR_TYPE, typename ntuple_type, typename firstarg, typename... arglist,
         typename std::enable_if<is_known<const typename firstarg::Ctype>::value &&
         TVAR_TYPE::Ctype::ID == firstarg::Ctype::ID, EI_type&>::type...>
constexpr auto iter_through_equations(const TVAR_TYPE& TARGETVAR_C, const ntuple_type& knownlist_C,
                                      const firstarg& current_eq_C, const arglist&... pack_C)
{
    return current_eq_C.unpack();
}

//firstarg is known, doesn't match TVAR's ID
template<typename TVAR_TYPE, typename ntuple_type, typename firstarg, typename... arglist,
         typename std::enable_if<is_known<const typename firstarg::Ctype>::value &&
         TVAR_TYPE::Ctype::ID != firstarg::Ctype::ID, EI_type&>::type...>
constexpr auto iter_through_equations(const TVAR_TYPE& TARGETVAR_C, const ntuple_type& knownlist_C,
                                      const firstarg& current_eq_C, const arglist&... pack_C)
{
    constexpr auto new_knownlist = (knownlist_C.unpack()).push_front(current_eq_C.unpack());

    using CXPR_new_knownlist = PACK(new_knownlist);

    return iter_through_equations(TARGETVAR_C, CXPR_new_knownlist{}, pack_C...);
}

#endif // CMD_LIST

/**
template<typename TVAR_TYPE, typename ntuple_type, typename firstarg, typename... arglist>
constexpr int iter_through_equations(const TVAR_TYPE& TARGETVAR, const ntuple_type& knownlist,
                                     const firstarg& current_eq, const arglist&... pack)
{
    const std::size_t depth = sizeof...(arglist);

    typedef firstarg current_type;

    if(is_known_failsafe<const current_type>::value)
    {
        if(current_eq.ID == TARGETVAR.ID)
        {
            return -1; //if value prob put in own func with same return type as this func
        }

        auto new_knownlist = knownlist.push_front(current_eq);

        //remove this eq from eqlist
    }

    if(std::is_same<current_type, bool>::value)
    {
        if(current_eq == 0)
        {
            return NAN;
        }

        //remove current eq from eqlist
    }

    if(is_generic_op_tree<firstarg>::value)
    {
        if(UK_count(current_eq) == 1)
        {
            //liberate hidden var, return resulting known

            auto var_inside = extract_known(current_eq);
            auto new_knownlist = knownlist.push_front(var_inside);

        }

        //bottom is for case that UK_count is not 1

        std::size_t num_of_args = knownlist.get_size();

        for(int a = 0; a<num_of_args; a++)
        {

            auto resultholder = new_information(current_eq, get<a>(knownlist) );
            if(is_known_failsafe<const decltype(resultholder)>::value)
            {
                //forward to self with updated current.
            }
            if(//is op tree)
            {
                update this and put it in back of tracklist. continue as normal.
            }

        }

        //plug in knownlist into current eq and record result. if known/bool, direct to
        //recursive function by calling this function with exact same args with modified firstarg.
        //else, record this by plugging in as last arg

    }

    return 4;
}

*/
