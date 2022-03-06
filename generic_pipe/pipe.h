#ifndef PIPE_H
#define PIPE_H

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
} // namespace pipe

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
        return call_rhs(lhs_(std::forward<T>(args)...));
    }

private:
    L lhs_;
    R rhs_;

    template<typename... T> constexpr auto call_rhs(T&&... args) const
    {
        return rhs_(std::forward<T>(args)...);
    }
};

template<typename L, typename R> constexpr auto operator|(L&& lhs, R&& rhs)
{
    return generic_pipe<L, R>(std::forward<L>(lhs), std::forward<R>(rhs));
}

#endif