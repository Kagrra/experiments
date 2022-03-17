#ifndef STATIC_STRING_H
#define STATIC_STRING_H

#include <algorithm>
#include <string_view>

template<size_t Size> struct static_string
{
    constexpr static_string(const char (&str)[Size])
    {
        std::copy_n(str, Size, data);
    };

    constexpr auto operator<=>(const static_string&) const = default;

    std::string_view constexpr get() const
    {
        return {&(data[0]), Size};
    }
    char data[Size] = {};
};

template<size_t N> static_string(const char (&data)[N]) -> static_string<N>;

template<static_string Name, typename T> struct named_counter
{
    using type = T;
    constexpr static_string name = Name;

    constexpr std::string_view name() const noexcept
    {
        return Name.get();
    }

    constexpr void inc() noexcept
    {
        count++;
    }
    int count = 0;
};

template<typename... T> struct counter : T...
{
public:
    constexpr counter() { }

    template<static_string Name> constexpr int inc() noexcept
    {
        return inc_impl_name<Name>(*this);
    }

    template<typename Type> constexpr int inc() noexcept
    {
        return inc_impl_type<Type>(*this);
    }

private:
    template<static_string Name, typename U> constexpr int inc_impl_name(named_counter<Name, U>& n)
    {
        return n.inc();
    }

    template<typename U, static_string Name> constexpr int inc_impl_type(named_counter<Name, U>& n)
    {
        return n.inc();
    }
};
#endif