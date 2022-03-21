#ifndef META_STRUCT_H
#define META_STRUCT_H

#include "named_type.h"

template<typename... T> class meta_struct : T...
{
    constexpr meta_struct() = default;
};

template<static_string Name, typename... Members> constexpr auto& get(meta_struct<Members...>& m) noexcept
{
    auto cast = []<static_string N, typename U>(named_type<N, U>& n) {
        return n;
    };
    return cast<Name>(m);
}

template<typename Fun, typename... Members> constexpr decltype(auto) apply(Fun&& fun, counter<Members...>& c)
{
    return std::forward<Fun>(fun)(static_cast<Members&>(c)...);
}

template<typename Fun, typename... Members> constexpr void for_each(Fun&& fun, counter<Members...>& c)
{
    auto sink = []<typename... Sink>(Sink... s)->void {};
    sink((std::forward<Fun>(fun)(static_cast<Members&>(c)), 0)...);
}

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

template<static_string Name, typename T> struct get_type
{
    using type = decltype(details::get_type_impl<Name, T>())::type;
};
template<static_string Name, typename T> using get_type_t = get_type<Name, T>::type;

#endif