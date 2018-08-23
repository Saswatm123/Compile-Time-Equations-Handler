#ifndef CT_counter
#define CT_counter

namespace count_impl
{
    template<int A>
    struct state
    {
        friend constexpr const std::size_t ADLfunc(state<A>);
    };

    template<std::size_t A>
    struct bit_alter
    {
        friend constexpr const std::size_t ADLfunc(state<A>)
        {
            return A;
        }

        static constexpr const std::size_t value = A;
    };

    template<int A, int = ADLfunc(state<A>{})>
    constexpr std::size_t find_base(int ignore, state<A>)
    {
        return A;
    }

    template<int A>
    constexpr std::size_t find_base(double ignore, state<A>, int R = find_base(0, state<A-1>{}) )
    {
        return R;
    }

    constexpr int find_base(double ignore, state<-1>)
    {
        return -1;
    }
}

template<int inc = 1>
constexpr std::size_t ticket(int val = count_impl::bit_alter<
                      count_impl::find_base(0, count_impl::state<BITS_Q_STATES>{} )+inc
                                                             >::value
                             )
{
    return val;
}

#endif // CT_counter
