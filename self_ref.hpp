#ifndef SELF_REF
#define SELF_REF

template<typename T>
class self_ref
{
private:
    self_ref() = default;
    friend T;
public:

    typedef T value_type;

    constexpr inline const T& getref() const
    {
        return static_cast<const T&>(*this);
    }
};

#endif // SELF_REF
