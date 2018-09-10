#ifndef CMD_LIST
#define CMD_LIST

#include "void_t.hpp"
#include "innertype_calc.hpp"
#include "ntuple.hpp"
#include "EIF_filler.hpp"

#include<type_traits>

template<typename T, class = void>
struct CMDLIST_restrictor
{typedef typename std::enable_if<is_generic_op_tree<T>::value, T>::type type;};

template<typename T>
struct CMDLIST_restrictor<T, void_t<typename std::enable_if<is_known<const T>::value, T>::type> >
{typedef T type;};

template<>
struct CMDLIST_restrictor<bool>
{typedef bool type;};

template<typename op_tree_piece>
inline constexpr const
typename std::enable_if<is_unknown<const op_tree_piece>::value, std::size_t>::type
UK_count(const op_tree_piece& OT, std::size_t count = 0)
{
    return count + 1;
}

template<typename op_tree_piece>
inline constexpr const
typename std::enable_if<is_generic_op_tree<const op_tree_piece>::value, std::size_t>::type
UK_count(const op_tree_piece& OT, std::size_t count = 0)
{
    return UK_count(OT.left, count) + UK_count(OT.right, count);
}

template<typename op_tree_piece>
inline constexpr const
typename std::enable_if<!is_generic_op_tree<const op_tree_piece>::value && !is_unknown_failsafe<const op_tree_piece>::value, std::size_t>::type
UK_count(const op_tree_piece& OT, std::size_t count = 0)
{
    return count;
}

inline constexpr const bool has_noted_ID(const long double& OT, const int& ID)
{
    return false;
}

template<typename is_unknown_var, typename std::enable_if<is_unknown<const is_unknown_var>::value, is_unknown_var>::type...>
inline constexpr const bool has_noted_ID(const is_unknown_var& OT, const int& ID)
{
    if(OT.ID == ID)
    {
        return true;
    }
    return false;
}

template<typename is_op_tree, typename std::enable_if<is_generic_op_tree<const is_op_tree>::value, is_op_tree>::type...>
inline constexpr const bool has_noted_ID(const is_op_tree& OT, const int& ID)
{
    return has_noted_ID(OT.left, ID) || has_noted_ID(OT.right, ID);
}

template<typename is_op_tree, typename newinf_t,
typename std::enable_if<is_generic_op_tree<const is_op_tree>::value &&
                        is_known_failsafe<const newinf_t>::value, is_op_tree>::type...>
inline constexpr const auto new_information(const is_op_tree& basetree, const newinf_t& newinf)
{
    //NI it and UKcount check afterwards. if UKcount afterwards
    //results in 1, eval numerically. Return type splitter
    return is_op_tree::template NI_op_tree<is_op_tree, newinf_t, has_noted_ID(basetree, newinf.ID)>(basetree, newinf);
}

///ASSUMES NO K==C SINCE SIMPLIFIES TO BOOL. FORM IS f(K)==C
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

    //SE should assume operation is 0-3 (no equals, have handler filter out)
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
    }

    namespace resolve_impl
    {
        //forward declarations

        template<typename is_op_tree, typename std::enable_if<std::is_arithmetic<typename is_op_tree::Rtype>::value &&
                                               !std::is_arithmetic<typename is_op_tree::Ltype>::value, EI_type>::type...>
        inline constexpr const long double
        resolve(const is_op_tree&);

        template<typename is_op_tree, typename std::enable_if<std::is_arithmetic<typename is_op_tree::Ltype>::value &&
                                               !std::is_arithmetic<typename is_op_tree::Rtype>::value, EI_type>::type...>
        inline constexpr const long double
        resolve(const is_op_tree&);

        template<typename is_op_tree,
                 typename std::enable_if<!std::is_arithmetic<typename is_op_tree::Ltype>::value &&
                                         !std::is_arithmetic<typename is_op_tree::Rtype>::value,
                                         EI_type
                                         >::type...
                 >
        inline constexpr const long double
        resolve(const is_op_tree&);

        template<typename is_op_tree, typename std::enable_if<std::is_arithmetic<typename is_op_tree::Ltype>::value &&
                                                              std::is_arithmetic<typename is_op_tree::Rtype>::value,
                                                              EI_type
                                                              >::type...
                 >
        inline constexpr const long double
        resolve(const is_op_tree&);

        inline constexpr const long double
        resolve(const long double);

        /**
        *   Take in op_tree of all ops and constants and
        *   simplify to one constant. Variants are C-C (base case),
        *   C-O, O-C, and O-O. Called from EKM in order to simplify
        *   side branch
        */

        inline constexpr const long double
        resolve(const long double val)
        {
            return val;
        }

        template<typename is_op_tree, typename std::enable_if<std::is_arithmetic<typename is_op_tree::Ltype>::value &&
                                                              std::is_arithmetic<typename is_op_tree::Rtype>::value,
                                                              EI_type
                                                              >::type...
                 >
        inline constexpr const long double
        resolve(const is_op_tree& OT)
        {
            /*
            static_assert(std::is_same<const op_tree<long double, long double>&, decltype(OT) >::value, "");

            static_assert(std::is_same<const int&, std::add_const< int >::type& >::value, "");*/

            ///prob is passing in OT.operation as template argument (not guaranteed as constexpr arg)
            return short_eval(OT.operation, OT.left, OT.right);
        }

        template<typename is_op_tree, typename std::enable_if<std::is_arithmetic<typename is_op_tree::Rtype>::value &&
                                               !std::is_arithmetic<typename is_op_tree::Ltype>::value, EI_type>::type...>
        inline constexpr const long double
        resolve(const is_op_tree& OT)
        {
            /*
            static_assert(std::is_arithmetic<typename is_op_tree::Ltype::Rtype>::value &&
                                               !std::is_arithmetic<typename is_op_tree::Ltype::Ltype>::value, "");*/

            return short_eval(OT.operation, resolve(OT.left), OT.right);
        }

        template<typename is_op_tree, typename std::enable_if<std::is_arithmetic<typename is_op_tree::Ltype>::value &&
                                               !std::is_arithmetic<typename is_op_tree::Rtype>::value, EI_type>::type...>
        inline constexpr const long double
        resolve(const is_op_tree& OT)
        {
            return short_eval(OT.operation, OT.left, resolve(OT.right) );
        }

        template<typename is_op_tree,
                 typename std::enable_if<!std::is_arithmetic<typename is_op_tree::Ltype>::value &&
                                         !std::is_arithmetic<typename is_op_tree::Rtype>::value,
                                         EI_type
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
                 typename std::enable_if<M_tree == OpType<char,char>::add, EI_type
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
                 typename std::enable_if<M_tree == OpType<char,char>::sub && !side2, EI_type>::type...
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

            /*
            auto new_M_tree = (side2?(side1?OT.right:OT.left).right
                                    :(side1?OT.right:OT.left).left);*/

            auto ret_help_left  = ternary<!side1, decltype(new_M_tree), decltype(S_tree)>(new_M_tree, S_tree);
            auto ret_help_right = ternary< side1, decltype(new_M_tree), decltype(S_tree)>(new_M_tree, S_tree);

            return op_tree<decltype(ret_help_left), decltype(ret_help_right)>
                          (OpType<decltype(ret_help_left), decltype(ret_help_right)>::equal,
                           ret_help_left, ret_help_right);
            /*
            return op_tree<decltype(side1?new_M_tree:S_tree), decltype(!side1?new_M_tree:S_tree)>
                          (OpType<decltype(side1?new_M_tree:S_tree), decltype(!side1?new_M_tree:S_tree)>::equal,
                           side1?new_M_tree:S_tree, (!side1)?new_M_tree:S_tree);
                           */
        }

        ///sub, MRight
        template<int M_tree, bool side1, bool side2, typename is_op_tree,
                 typename std::enable_if<M_tree == OpType<char,char>::sub && side2, EI_type
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
                 typename std::enable_if<M_tree == OpType<char,char>::mult, EI_type
                                         >::type...
                 >
        inline constexpr const auto
        extract_known(const is_op_tree& OT)
        {
            /*
            auto S_branch = ((!side1)?OT.right:OT.left); //non-important side

            auto M_branch = ((!side2)?(side1?OT.right:OT.left).right
                                     :(side1?OT.right:OT.left).left); //clip to opp. side

            auto S_tree = op_tree<decltype(S_branch),decltype(M_branch)>
            (OpType<decltype(S_branch),decltype(M_branch)>::div, S_branch, M_branch);

            auto new_M_tree = (side2?(side1?OT.right:OT.left).right
                                    :(side1?OT.right:OT.left).left);

            return op_tree<decltype(side1?new_M_tree:S_tree), decltype((!side1)?new_M_tree:S_tree)>
                          (OpType<decltype(side1?new_M_tree:S_tree), decltype((!side1)?new_M_tree:S_tree)>::equal,
                           side1?new_M_tree:S_tree, (!side1)?new_M_tree:S_tree);
            */
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
                 typename std::enable_if<M_tree == OpType<char,char>::div && !side2, EI_type
                                         >::type...
                 >
        inline constexpr const auto
        extract_known(const is_op_tree& OT)
        {
            /*
            auto S_branch = ((!side1)?OT.right:OT.left); //non-important side

            auto M_branch = ((!side2)?(side1?OT.right:OT.left).right
                                     :(side1?OT.right:OT.left).left); //clip to opp. side

            auto S_tree = op_tree<decltype(S_branch), decltype(M_branch)>
            (OpType<decltype(S_branch), decltype(M_branch)>::mult, S_branch, M_branch);

            auto new_M_tree = (side2?(side1?OT.right:OT.left).right
                                    :(side1?OT.right:OT.left).left);

            return op_tree<decltype(side1?new_M_tree:S_tree), decltype(!side1?new_M_tree:S_tree)>
                          (OpType<decltype(side1?new_M_tree:S_tree), decltype(!side1?new_M_tree:S_tree)>::equal,
                           side1?new_M_tree:S_tree, (!side1)?new_M_tree:S_tree);
            */

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
                 typename std::enable_if<M_tree == OpType<char,char>::div && side2, EI_type>::type...
                 >
        inline constexpr const auto
        extract_known(const is_op_tree& OT)
        {
            /*
            using op_base = op_tree<typename is_op_tree::Ltype, typename is_op_tree::Rtype>;

            auto S_branch = ((!side1)?OT.right:OT.left); //non-important side

            auto M_branch = ((!side2)?(side1?OT.right:OT.left).right
                                     :(side1?OT.right:OT.left).left); //clip to opp. side

            auto mid_RHS = op_tree<decltype(S_branch),decltype(M_branch)>
                                  (OpType<decltype(S_branch),decltype(M_branch)>::div, S_branch, M_branch);

            auto final_S_tree = op_tree<decltype(mid_RHS), long double>
                                (OpType<decltype(mid_RHS), long double>::div, 1, mid_RHS);

            auto new_M_tree = (side2?(side1?OT.right:OT.left).right
                                    :(side1?OT.right:OT.left).left);

            return op_tree<decltype(side1?new_M_tree:final_S_tree), decltype(!side1?new_M_tree:final_S_tree)>
                          (OpType<decltype(side1?new_M_tree:final_S_tree), decltype(!side1?new_M_tree:final_S_tree)>::equal,
                           side1?new_M_tree:final_S_tree, !side1?new_M_tree:final_S_tree);*/

            //non-important side
            auto S_branch = ternary<!side1, decltype(OT.right), decltype(OT.left)>(OT.right, OT.left);

            auto helper_left  = ternary<side1, decltype(OT.right), decltype(OT.left)>(OT.right, OT.left).right;
            auto helper_right = ternary<side1, decltype(OT.right), decltype(OT.left)>(OT.right, OT.left).left ;

            //clip to opp. side
            auto M_branch = ternary<!side2, decltype(helper_left), decltype(helper_right)>(helper_left, helper_right);

            auto mid_RHS = op_tree<decltype(S_branch),decltype(M_branch)>
                                  (OpType<decltype(S_branch),decltype(M_branch)>::div, S_branch, M_branch);

            auto final_S_tree = op_tree<decltype(mid_RHS), long double>
                                (OpType<decltype(mid_RHS), long double>::div, 1, mid_RHS);

            auto helper_1 = ternary<side1, decltype(OT.right), decltype(OT.left)>(OT.right, OT.left).right;
            auto helper_2 = ternary<side1, decltype(OT.right), decltype(OT.left)>(OT.right, OT.left).left ;

            auto new_M_tree = ternary<side2, decltype(helper_1), decltype(helper_2)>(helper_1, helper_2);

            auto ret_help_left  = ternary<!side1, decltype(new_M_tree), decltype(final_S_tree)>(new_M_tree, final_S_tree);
            auto ret_help_right = ternary< side1, decltype(new_M_tree), decltype(final_S_tree)>(new_M_tree, final_S_tree);

            return op_tree<decltype(ret_help_left), decltype(ret_help_right)>
                          (OpType<decltype(ret_help_left), decltype(ret_help_right)>::equal,
                           ret_help_left, ret_help_right);
        }
    }

    namespace EKM_impl
    {
        //only called from EKM, returns tree to remove
        template<bool B, typename first, typename second, typename std::enable_if<B, EI_type>::type...>
        inline constexpr const auto
        M_tree_decider(const first& first_tree, const second& second_tree)
        {
            return first_tree;
        }

        template<bool B, typename first, typename second, typename std::enable_if<!B, EI_type>::type...>
        inline constexpr const auto
        M_tree_decider(const first& first_tree, const second& second_tree)
        {
            return second_tree;
        }
    }

    //UK = f(C)
    template<typename is_op_tree, typename std::enable_if<is_unknown_failsafe<const typename is_op_tree::Ltype>::value &&
                                                          !is_unknown_failsafe<const typename is_op_tree::Rtype>::value,
                                                          EI_type
                                                          >::type...
             >
    inline constexpr const var<is_op_tree::Ltype::ID, 1>
    extract_known_main(const is_op_tree& OT)
    {
        //non UK side
        auto N = EKM_impl::M_tree_decider<is_unknown_failsafe<const typename is_op_tree::Ltype>::value,
                                          decltype(OT.right), decltype(OT.left)
                                          >(OT.right, OT.left);

        auto C_from_func_C = resolve_impl::resolve(N);

        return short_eval(OT.operation, OT.left, C_from_func_C);
    }

    //f(C) = UK
    template<typename is_op_tree, typename std::enable_if<!is_unknown_failsafe<const typename is_op_tree::type::Ltype>::value &&
                                                          is_unknown_failsafe<const typename is_op_tree::type::Rtype>::value,
                                                          EI_type
                                                          >::type...
             >
    inline constexpr const var<is_op_tree::type::Rtype::ID, 1>
    extract_known_main(const is_op_tree& OT)
    {
        //non UK side
        auto N = EKM_impl::M_tree_decider<is_unknown_failsafe<const typename is_op_tree::Ltype>::value,
                                          decltype(OT.value().right), decltype(OT.value().left)
                                          >(OT.value().right, OT.value().left);

        auto C_from_func_C = resolve_impl::resolve(N);

        return short_eval(OT.operation, C_from_func_C, OT.right);
    }

    //assumes only 1 UK whole tree
    template<typename is_op_tree, typename std::enable_if<!is_unknown_failsafe<const typename is_op_tree::type::Ltype>::value &&
                                                          !is_unknown_failsafe<const typename is_op_tree::type::Rtype>::value,
                                                          EI_type
                                                          >::type...
             >
    inline constexpr const auto
    extract_known_main(const is_op_tree& OT)
    {
        constexpr std::size_t side = UK_count(OT.value().right); //0 left 1 right

        constexpr auto make_not_prvalue = ternary<side>(OT.value().right, OT.value().left).right;

        constexpr std::size_t d_side = UK_count(make_not_prvalue);

        //node to remove
        constexpr auto M_tree = EKM_impl::M_tree_decider<side>
                      (OT.value().right, OT.value().left);

        //tree with most upcoming operand inverted
        auto passdown = extract_impl::extract_known<M_tree.operation, side, d_side>(OT.value());

        return extract_known_main(passdown); //Explicitly handles all "equals" cases, no passdown has equals
    }
}

//Plug In Knowns
namespace PIK
{
    using namespace extract_detail;

    template<typename is_op_tree, typename ntup_type, std::size_t UK_num, std::size_t maxind, std::size_t ind,
             typename std::enable_if<UK_num==1 && ind==(maxind-1), EI_type>::type...>
    inline constexpr const auto
    plug_in_knowns(const is_op_tree& OT, const ntup_type& knownlist)
    {
        return extract_known_main(OT);
    }

    template<typename is_op_tree, typename ntup_type, std::size_t UK_num, std::size_t maxind, std::size_t ind,
             typename std::enable_if<UK_num==1 && ind!=(maxind-1), EI_type>::type...>
    inline constexpr const auto
    plug_in_knowns(const is_op_tree& OT, const ntup_type& knownlist)
    {
        return extract_known_main(OT);
    }

    template<typename is_op_tree, typename ntup_type, std::size_t UK_num, std::size_t maxind, std::size_t ind,
             typename std::enable_if<UK_num!=1 && ind==(maxind-1), EI_type>::type...>
    inline constexpr const auto
    plug_in_knowns(const is_op_tree& OT, const ntup_type& knownlist)
    {
        return new_information(OT, get<ind>(knownlist));
    }

    template<typename is_op_tree, typename ntup_type, std::size_t UK_num, std::size_t maxind, std::size_t ind,
             typename std::enable_if<UK_num!=1 && ind!=(maxind-1), EI_type>::type...>
    inline constexpr const auto
    plug_in_knowns(const is_op_tree& OT, const ntup_type& knownlist)
    {
        auto Q = new_information(OT, get<ind>(knownlist));
        return plug_in_knowns<decltype(Q), decltype(knownlist), UK_count(Q), maxind, ind+1>
        (Q, knownlist);
    }
}

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
            /*
            auto var_inside = extract_known(current_eq);
            auto new_knownlist = knownlist.push_front(var_inside);
            */
        }

        //bottom is for case that UK_count is not 1

        std::size_t num_of_args = knownlist.get_size();

        for(int a = 0; a<num_of_args; a++)
        {
            /*
            auto resultholder = new_information(current_eq, get<a>(knownlist) );
            if(is_known_failsafe<const decltype(resultholder)>::value)
            {
                //forward to self with updated current.
            }
            if(//is op tree)
            {
                update this and put it in back of tracklist. continue as normal.
            }
            */
        }

        //plug in knownlist into current eq and record result. if known/bool, direct to
        //recursive function by calling this function with exact same args with modified firstarg.
        //else, record this by plugging in as last arg

    }

    return 4;
}

#endif // CMD_LIST
