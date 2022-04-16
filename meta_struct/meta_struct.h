#ifndef META_STRUCT_H
#define META_STRUCT_H

#include "named_type.h"

template<typename... T> class meta_struct : public T...
{
public:
    constexpr meta_struct() = default;

    template<typename... U>
    constexpr meta_struct(U... args)
        : T(std::move(args))...
    {
    }
};

template<static_string Name, typename T> constexpr decltype(auto) get(T&& m) noexcept
{
    return []<typename U>(named_type<Name, U>& n) -> decltype(auto) {
        return n;
    }(std::forward<T>(m));
}

template<size_t Index, typename... Members> constexpr auto& get(meta_struct<Members...>& m) noexcept
{
    // TODO
}

template<typename Fun, typename... Members> constexpr decltype(auto) apply(Fun&& fun, meta_struct<Members...>& c)
{
    return std::forward<Fun>(fun)(static_cast<Members&>(c)...);
}

template<typename Fun, typename... Members> constexpr void for_each(Fun&& fun, meta_struct<Members...>& c)
{
    ((std::forward<Fun>(fun)(static_cast<Members&>(c))), ...);
}

namespace details
{
template<static_string Name, typename T> constexpr auto get_type_impl()
{
    return [&]<typename U>(named_type<Name, U>* n) {
        struct ret
        {
            using type = U;
        };
        return ret {};
    }(static_cast<T*>(nullptr));
}
} // namespace details

template<static_string Name, typename T> struct get_type
{
    using type = typename decltype(details::get_type_impl<Name, T>())::type;
};
template<static_string Name, typename T> using get_type_t = typename get_type<Name, T>::type;

template<static_string Name, typename T> concept has_name = requires(T m)
{
    get<Name>(m);
};

#endif