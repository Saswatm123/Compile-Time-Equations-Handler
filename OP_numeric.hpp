#ifndef OP_NUMERIC
#define OP_NUMERIC

/**
*   Various numeric calculations pertaining to op trees, such as # of unknowns,
*   whether a target unknown is inside the tree, etc.
*/

//fwd declarations of UK_count

template<typename op_tree_piece>
inline constexpr const
typename std::enable_if<!is_generic_op_tree<const op_tree_piece>::value && !is_unknown_failsafe<const op_tree_piece>::value, std::size_t>::type
UK_count(const op_tree_piece&, std::size_t = 0);

template<typename op_tree_piece>
inline constexpr const
typename std::enable_if<is_generic_op_tree<const op_tree_piece>::value, std::size_t>::type
UK_count(const op_tree_piece&, std::size_t = 0);

template<typename op_tree_piece>
inline constexpr const
typename std::enable_if<is_unknown_failsafe<const op_tree_piece>::value, std::size_t>::type
UK_count(const op_tree_piece&, std::size_t = 0);

//End fwd declarations, begin definitions

template<typename op_tree_piece>
inline constexpr const
typename std::enable_if<is_unknown_failsafe<const op_tree_piece>::value, std::size_t>::type
UK_count(const op_tree_piece& OT, std::size_t count)
{
    return count + 1;
}

template<typename op_tree_piece>
inline constexpr const
typename std::enable_if<is_generic_op_tree<const op_tree_piece>::value, std::size_t>::type
UK_count(const op_tree_piece& OT, std::size_t count)
{
    return UK_count(OT.left, count) + UK_count(OT.right, count);
}

// !op_tree && !unknown, includes constant, unary_ftype
template<typename op_tree_piece>
inline constexpr const
typename std::enable_if<!is_generic_op_tree<const op_tree_piece>::value && !is_unknown_failsafe<const op_tree_piece>::value, std::size_t>::type
UK_count(const op_tree_piece& OT, std::size_t count)
{
    return count;
}

//end def, begin def for type safe version

template<typename op_tree_piece, std::size_t count = 0>
inline constexpr const
typename std::enable_if<is_unknown_failsafe<const op_tree_piece>::value, std::size_t>::type
UK_count_TS()
{
    return count + 1;
}

template<typename op_tree_piece, std::size_t count = 0>
inline constexpr const
typename std::enable_if<!is_generic_op_tree<const op_tree_piece>::value && !is_unknown_failsafe<const op_tree_piece>::value, std::size_t>::type
UK_count_TS()
{
    return count;
}

template<typename op_tree_piece, std::size_t count = 0>
inline constexpr const
typename std::enable_if<is_generic_op_tree<const op_tree_piece>::value, std::size_t>::type
UK_count_TS()
{
    return UK_count_TS<typename op_tree_piece::Ltype, count>() +
           UK_count_TS<typename op_tree_piece::Rtype, count>();
}

//end def, begin def for ID search

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

template<typename is_op_tree, typename std::enable_if<is_unary_ftype<const is_op_tree>::value, is_op_tree>::type...>
inline constexpr const bool has_noted_ID(const is_op_tree& OT, const int& ID)
{
    return false;
}

#endif // OP_NUMERIC
