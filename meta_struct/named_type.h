#ifndef NAMED_TYPE_H
#define NAMED_TYPE_H

#include "static_string.h"

template<static_string Name, typename T> class named_type
{
public:
    using type = T;

    constexpr named_type() = default;

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
#endif