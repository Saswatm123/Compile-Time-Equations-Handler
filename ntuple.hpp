#ifndef NTUPLE
#define NTUPLE

#include "pack.hpp"

template<typename... emptylist>
struct ntuple
{
    inline static constexpr const std::size_t get_size(std::size_t N)
    {
        return (N+1);
    }
};

template<typename firstarg, typename... arglist>
struct ntuple<firstarg, arglist...> : ntuple<arglist...>
{
    constexpr ntuple(firstarg arg1, arglist... pack)
    :ntuple<arglist...>(pack...), obj(arg1)
    {}

    template<typename...inp_arglist>
    constexpr ntuple(const firstarg& new_arg, const ntuple<inp_arglist...>& basetuple)
    :obj(new_arg), ntuple<arglist...>(basetuple)
    {}

    template<typename newtype>
    constexpr ntuple<newtype, firstarg, arglist...>
    push_front(const newtype& new_arg) const
    {
        return ntuple<newtype, firstarg, arglist...>(new_arg, *this);
    }

    inline static constexpr const std::size_t get_size(std::size_t N = 0)
    {
        return ntuple<arglist...>::get_size(N+1);
    }

    typedef PACK::pack<arglist...> fwd_types;

    typedef firstarg type;

    firstarg obj;
};

namespace typeholder_impl
{
    template<size_t N, typename firstarg, class...argpack>
    struct typeholder
    {
        typedef typename typeholder<N-1, argpack...>::type type;
    };

    template<typename firstarg, class...argpack>
    struct typeholder<0,firstarg,argpack...>
    {
        typedef firstarg type;
    };
}

template <std::size_t k, class...Ts>
constexpr const
typename std::enable_if<k==0, typename typeholder_impl::typeholder<0, Ts...>::type>::type&
get(const ntuple<Ts...>& t)
{
    return t.obj;
}

template <std::size_t k, class T, class...Ts>
constexpr const
typename std::enable_if<k!=0, typename typeholder_impl::typeholder<k, T, Ts...>::type>::type&
get(const ntuple<T,Ts...>& t)
{
    const ntuple<Ts...>& base = t;
    return get<k-1>(base);
}

#endif // NTUPLE
