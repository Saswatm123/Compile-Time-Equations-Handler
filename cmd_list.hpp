#ifndef CMD_LIST
#define CMD_LIST

#include "void_t.hpp"
#include "innertype_calc.hpp"
#include "ntuple.hpp"
#include "EIF_filler.hpp"

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
typename std::enable_if<!is_generic_op_tree<const op_tree_piece>::value && !is_unknown<const op_tree_piece>::value, std::size_t>::type
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

template<typename is_op_tree, typename newinf_t, typename std::enable_if<is_generic_op_tree<const is_op_tree>::value, is_op_tree>::type...>
inline constexpr const auto new_information(const is_op_tree& basetree, const newinf_t& newinf)
{
    //NI it and UKcount check afterwards. if UKcount afterwards
    //results in 1, eval numerically.
    return 1;
}

///ASSUMES NO K==C SINCE SIMPLIFIES TO BOOL. FORM IS f(K)==C
namespace extract_detail
{
    template<typename is_opr>
    inline constexpr const long double short_eval(const is_opr& operation, const long double lhs, const long double rhs)
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
        if(operation == OP::equal)
        {
            return lhs==rhs;
        }
    }

    namespace resolve_impl
    {
        template<typename is_op_tree,
                 typename std::enable_if<std::is_arithmetic<typename is_op_tree::Rtype>::value &&
                                        !std::is_arithmetic<typename is_op_tree::Ltype>::value, EI_type>::type...>
        inline constexpr const long double resolve(const is_op_tree& OT)
        {
            return short_eval(OT.operation, resolve(OT.left), OT.right);
        }

        template<typename is_op_tree,
                 typename std::enable_if<std::is_arithmetic<typename is_op_tree::Ltype>::value &&
                                        !std::is_arithmetic<typename is_op_tree::Rtype>::value, EI_type>::type...>
        inline constexpr const long double resolve(const is_op_tree& OT)
        {
            return short_eval(OT.operation, OT.left, resolve(OT.right) );
        }

        template<typename is_op_tree, typename std::enable_if<std::is_arithmetic<typename is_op_tree::Ltype>::value &&
                                                              std::is_arithmetic<typename is_op_tree::Rtype>::value,
                                                              EI_type
                                                              >::type...
                 >
        inline constexpr const long double resolve(const is_op_tree& OT)
        {
            return short_eval(OT.operation, OT.left, OT.right);
        }

        template<typename is_op_tree, typename std::enable_if<!std::is_arithmetic<typename is_op_tree::Ltype>::value &&
                                                              !std::is_arithmetic<typename is_op_tree::Rtype>::value,
                                                              EI_type
                                                              >::type...
                 >
        inline constexpr const long double resolve(const is_op_tree& OT)
        {
            return short_eval(OT.operation, resolve(OT.left), resolve(OT.right));
        }

    }

    namespace extract_impl
    {
        ///add
        template<typename is_op_tree, typename M_op_type, M_op_type M_tree, bool side1, bool side2,
                 typename std::enable_if<M_tree.operation == OpType<
                                                                    typename M_op_type::Ltype,
                                                                    typename M_op_type::Rtype
                                                                    >::add, EI_type
                                         >::type...
                 >
        inline constexpr const auto extract_known(const is_op_tree& OT)
        {
            auto S_branch = ((!side1)?OT.right:OT.left); //non-important side

            auto M_branch = ((!side2)?(side1?OT.right:OT.left).right
                                     :(side1?OT.right:OT.left).left); //clip to opp. side

            auto S_tree = op_tree<decltype(S_branch),decltype(M_branch)>
            (OpType<decltype(S_branch),decltype(M_branch)>::sub, S_branch, M_branch);

            auto new_M_tree = (side2?(side1?OT.right:OT.left).right
                                    :(side1?OT.right:OT.left).left);

            return op_tree<decltype(side1?new_M_tree:S_tree), decltype((!side1)?new_M_tree:S_tree)>
                          (OpType<decltype(side1?new_M_tree:S_tree), decltype((!side1)?new_M_tree:S_tree)>::equal,
                           side1?new_M_tree:S_tree, (!side1)?new_M_tree:S_tree);
        }

        ///sub, MLeft
        template<typename is_op_tree, typename M_op_type, M_op_type M_tree, bool side1, bool side2,
                 typename std::enable_if<M_tree.operation == OpType<
                                                                    typename M_op_type::Ltype,
                                                                    typename M_op_type::Rtype
                                                                    >::sub && !side2, EI_type
                                         >::type...
                 >
        inline constexpr const auto extract_known(const is_op_tree& OT)
        {
            auto S_branch = ((!side1)?OT.right:OT.left); //non-important side

            auto M_branch = ((!side2)?(side1?OT.right:OT.left).right
                                     :(side1?OT.right:OT.left).left); //clip to opp. side

            //if side2==1, clip(sub,S,M) -> clip(mult, prev, -1)
            //if side2==0, clip(add,S,M)

            auto S_tree = op_tree<decltype(S_branch), decltype(M_branch)>
            (OpType<decltype(S_branch), decltype(M_branch)>::add, S_branch, M_branch);

            auto new_M_tree = (side2?(side1?OT.right:OT.left).right
                                    :(side1?OT.right:OT.left).left);

            return op_tree<decltype(side1?new_M_tree:S_tree), decltype(!side1?new_M_tree:S_tree)>
                          (OpType<decltype(side1?new_M_tree:S_tree), decltype(!side1?new_M_tree:S_tree)>::equal,
                           side1?new_M_tree:S_tree, (!side1)?new_M_tree:S_tree);
        }

        ///sub, MRight
        template<typename is_op_tree, typename M_op_type, M_op_type M_tree, bool side1, bool side2,
                 typename std::enable_if<M_tree.operation == OpType<
                                                                    typename M_op_type::Ltype,
                                                                    typename M_op_type::Rtype
                                                                    >::sub && side2, EI_type
                                         >::type...
                 >
        inline constexpr const auto extract_known(const is_op_tree& OT)
        {
            using op_base = op_tree<typename is_op_tree::Ltype, typename is_op_tree::Rtype>;

            auto S_branch = ((!side1)?OT.right:OT.left); //non-important side

            auto M_branch = ((!side2)?(side1?OT.right:OT.left).right
                                     :(side1?OT.right:OT.left).left); //clip to opp. side

            //if side2==1, clip(sub,S,M) -> clip(mult, prev, -1)

            auto mid_RHS = op_tree<decltype(S_branch),decltype(M_branch)>
                                  (OpType<decltype(S_branch),decltype(M_branch)>::sub, S_branch, M_branch);

            auto final_S_tree = op_tree<decltype(mid_RHS), long double>
                                (OpType<decltype(mid_RHS), long double>::mult, mid_RHS, -1);

            auto new_M_tree = (side2?(side1?OT.right:OT.left).right
                                    :(side1?OT.right:OT.left).left);

            return op_tree<decltype(side1?new_M_tree:final_S_tree), decltype(!side1?new_M_tree:final_S_tree)>
                          (OpType<decltype(side1?new_M_tree:final_S_tree), decltype(!side1?new_M_tree:final_S_tree)>::equal,
                           side1?new_M_tree:final_S_tree, !side1?new_M_tree:final_S_tree);
        }

        ///mult
        template<typename is_op_tree, typename M_op_type, M_op_type M_tree, bool side1, bool side2,
                 typename std::enable_if<M_tree.operation == OpType<
                                                                    typename M_op_type::Ltype,
                                                                    typename M_op_type::Rtype
                                                                    >::mult, EI_type
                                         >::type...
                 >
        inline constexpr const auto extract_known(const is_op_tree& OT)
        {
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
        }

        ///div, Mleft
        template<typename is_op_tree, typename M_op_type, M_op_type M_tree, bool side1, bool side2,
                 typename std::enable_if<M_tree.operation == OpType<
                                                                    typename M_op_type::Ltype,
                                                                    typename M_op_type::Rtype
                                                                    >::div && !side2, EI_type
                                         >::type...
                 >
        inline constexpr const auto extract_known(const is_op_tree& OT)
        {
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
        }

        ///div, MRight
        template<typename is_op_tree, typename M_op_type, M_op_type M_tree, bool side1, bool side2,
                 typename std::enable_if<M_tree.operation == OpType<
                                                                    typename M_op_type::Ltype,
                                                                    typename M_op_type::Rtype
                                                                    >::div && side2, EI_type
                                         >::type...
                 >
        inline constexpr const auto extract_known(const is_op_tree& OT)
        {
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
                           side1?new_M_tree:final_S_tree, !side1?new_M_tree:final_S_tree);
        }

    }

    template<typename is_op_tree, typename std::enable_if<is_unknown<const typename is_op_tree::Ltype>::value !=
                                                          is_unknown<const typename is_op_tree::Rtype>::value,
                                                          EI_type
                                                          >::type...
             >
    inline constexpr const long double extract_known_main(const is_op_tree& OT)
    {
        return short_eval(OpType<typename is_op_tree::Ltype, typename is_op_tree::Rtype>::equal, OT.left, OT.right);
    }

    template<typename is_op_tree, typename std::enable_if<!is_unknown<const typename is_op_tree::Ltype>::value &&
                                                          !is_unknown<const typename is_op_tree::Rtype>::value,
                                                          EI_type
                                                          >::type...
             >
    inline constexpr const long double extract_known_main(const is_op_tree& OT)
    {
        using namespace extract_impl;

        std::size_t side   = UK_count(OT.right); //0 left 1 right

        std::size_t d_side = UK_count((side?OT.right:OT.left).right);

        auto M_tree = side?OT.right:OT.left; //node to remove

        auto passdown = extract_known<decltype(OT), decltype(M_tree), M_tree, side, d_side>
                        (OT);

        return extract_known_main(passdown);
    }
}

//Plug In Knowns
namespace PIK
{
    using namespace extract_detail;

    template<typename is_op_tree, typename ntup_type, std::size_t UK_num, std::size_t maxind, std::size_t ind,
             typename std::enable_if<UK_num==1 && ind==(maxind-1), EI_type>::type...>
    inline constexpr const auto plug_in_knowns(const is_op_tree& OT, const ntup_type& knownlist)
    {
        return extract_known_main(OT);
    }

    template<typename is_op_tree, typename ntup_type, std::size_t UK_num, std::size_t maxind, std::size_t ind,
             typename std::enable_if<UK_num==1 && ind!=(maxind-1), EI_type>::type...>
    inline constexpr const auto plug_in_knowns(const is_op_tree& OT, const ntup_type& knownlist)
    {
        return extract_known_main(OT);
    }

    template<typename is_op_tree, typename ntup_type, std::size_t UK_num, std::size_t maxind, std::size_t ind,
             typename std::enable_if<UK_num!=1 && ind==(maxind-1), EI_type>::type...>
    inline constexpr const auto plug_in_knowns(const is_op_tree& OT, const ntup_type& knownlist)
    {
        return new_information(OT, get<ind>(knownlist));
    }

    template<typename is_op_tree, typename ntup_type, std::size_t UK_num, std::size_t maxind, std::size_t ind,
             typename std::enable_if<UK_num!=1 && ind!=(maxind-1), EI_type>::type...>
    inline constexpr const auto plug_in_knowns(const is_op_tree& OT, const ntup_type& knownlist)
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
