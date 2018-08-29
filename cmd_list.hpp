#ifndef CMD_LIST
#define CMD_LIST

#include "void_t.hpp"
#include "innertype_calc.hpp"
#include "ntuple.hpp"

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

template<typename is_op_tree>
constexpr const int extract_known(const is_op_tree& OT)
{

}

//Plug In Knowns
namespace PIK
{
    template<typename is_op_tree, typename ntup_type, std::size_t UK_num, std::size_t maxind, std::size_t ind,
             typename std::enable_if<UK_num==1 && ind==(maxind-1), int>::type...>
    inline constexpr const auto plug_in_knowns(const is_op_tree& OT, const ntup_type& knownlist)
    {
        return extract_known(OT);
    }

    template<typename is_op_tree, typename ntup_type, std::size_t UK_num, std::size_t maxind, std::size_t ind,
             typename std::enable_if<UK_num==1 && ind!=(maxind-1), int>::type...>
    inline constexpr const auto plug_in_knowns(const is_op_tree& OT, const ntup_type& knownlist)
    {
        return extract_known(OT);
    }

    template<typename is_op_tree, typename ntup_type, std::size_t UK_num, std::size_t maxind, std::size_t ind,
             typename std::enable_if<UK_num!=1 && ind==(maxind-1), int>::type...>
    inline constexpr const auto plug_in_knowns(const is_op_tree& OT, const ntup_type& knownlist)
    {
        return new_information(OT, get<ind>(knownlist));
    }

    template<typename is_op_tree, typename ntup_type, std::size_t UK_num, std::size_t maxind, std::size_t ind,
             typename std::enable_if<UK_num!=1 && ind!=(maxind-1), int>::type...>
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
