#ifndef NAMED_TYPE_H
#define NAMED_TYPE_H

#include "static_string.h"

template<static_string Name, typename T> class named_type
{
public:
    using type = T;

    constexpr named_type() = default;
    constexpr named_type(T obj)
        : value_ {obj}
    {
    }

    constexpr std::string_view name() const noexcept
    {
        return Name.get();
    }
    constexpr const T& value() const noexcept
    {
        return value_;
    }
    constexpr T& value() noexcept
    {
        return value_;
    }

    constexpr operator T() noexcept
    {
        return value();
    }

private:
    T value_ {};
};

template<static_string Name> struct arg_type
{
    template<typename T> constexpr decltype(auto) operator()(T value) const noexcept
    {
        return named_type<Name, T>(value);
    }
};

template<static_string Name> constexpr inline arg_type<Name> arg {};

#endif