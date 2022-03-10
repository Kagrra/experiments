#ifndef PIPE_H
#define PIPE_H

#include <functional>
#include <utility>

namespace pipe
{
struct start
{
    constexpr start() = default;

    template<typename T> constexpr auto operator()(T t) const
    {
        return t;
    }
};

template<typename T> struct constant
{
    constexpr constant(T val)
        : val_ {val}
    {
    }

    constexpr auto operator()() const
    {
        return val_;
    }
    const T val_;
};

template<typename... Fun> struct fork
{
    constexpr fork(Fun&&... fun)
        : fork_ {std::forward<Fun>(fun)...}
    {
    }

    template<typename... Args> constexpr auto operator()(Args&&... args) const
    {
        return fork_all_impl(std::make_index_sequence<std::tuple_size<decltype(fork_)>::value> {},
                             std::forward<Args>(args)...);
    }

private:
    template<std::size_t... I, typename... Args>
    constexpr auto fork_all_impl(std::index_sequence<I...> seq, Args... args) const
    {
        return std::tuple {std::invoke(std::get<I>(fork_), args...)...};
    }

    const std::tuple<Fun...> fork_;
};

template<typename L, typename R> class generic_pipe
{
public:
    constexpr generic_pipe(L lhs, R rhs)
        : lhs_ {lhs}
        , rhs_ {rhs}
    {
    }

    template<typename... T> constexpr auto operator()(T&&... args) const
    {
        if constexpr (std::is_invocable_v<R, std::invoke_result_t<L, T...>>)
            return rhs_(lhs_(std::forward<T>(args)...));
        else
            return std::apply(rhs_, lhs_(std::forward<T>(args)...));
    }

private:
    L lhs_;
    R rhs_;
};

template<typename L, typename R> constexpr auto operator|(L&& lhs, R&& rhs)
{
    return generic_pipe<L, R>(std::forward<L>(lhs), std::forward<R>(rhs));
}
} // namespace pipe
#endif