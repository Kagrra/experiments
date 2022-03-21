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

    constexpr std::string_view get() const
    {
        return {data, data + Size};
    }

    char data[Size] = {};
};

template<size_t Lhs, size_t Rhs> constexpr auto operator+(const static_string<Lhs>& lhs, const static_string<Rhs>& rhs)
{
    char data[Lhs + Rhs] = {};
    std::copy_n(lhs.data, Lhs, data);
    std::copy_n(rhs.data, Rhs, data + Lhs - 1);
    return static_string<Lhs + Rhs>(data);
}

template<size_t Value> constexpr auto itoa()
{
    auto value = Value;

    const size_t width = [&] {
        size_t i {1};
        while (std::pow(10, i) < Value) { ++i; }
        return i;
    }();

    char data[width] {};
    for (size_t i = 0; i < width; i++)
    {
        data[i] = static_cast<char>(value % 10 + '0');
        value /= 10;
    }
    std::reverse(&data[0], &data[width]);

    return static_string(data);
}

template<static_string Name, typename T> struct named_type
{
    using type = T;

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

namespace details
{
template<static_string Name, typename T> constexpr auto get_type_impl()
{
    auto lam = [&]<typename U>(const named_type<Name, U>& n) {
        struct ret
        {
            using type = U;
        };
        return ret {};
    };
    return lam(T {});
}
} // namespace details

template<static_string Name, typename T> using get_type = decltype(details::get_type_impl<Name, T>())::type;
#endif