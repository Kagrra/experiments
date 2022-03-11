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

#endif