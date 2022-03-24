#include <functional>
#include <type_traits>

template<typename Signature> struct callback;

template<typename R, typename... Args> struct callback<R(Args...)>
{
    using type = std::function<R(Args...)>;
    static auto no_op()
    {
        return [](Args...) -> R {
            if constexpr (!std::is_same_v<R, void>)
                return R {};
        };
    }
};

template<typename Base, typename R, typename... Args> //
struct callback<R (Base::*)(Args...)> : callback<R(Args...)>
{
};

template<typename Base, typename R, typename... Args> //
struct callback<R (Base::*)(Args...) const> : callback<R(Args...)>
{
};

template<typename Base, typename R, typename... Args> //
struct callback<R (Base::*)(Args...) noexcept> : callback<R(Args...)>
{
};

template<typename Base, typename R, typename... Args> //
struct callback<R (Base::*)(Args...) const noexcept> : callback<R(Args...)>
{
};

#define CALL(Base, Fun) callback<decltype(&Base::Fun)>::type on_##Fun = callback<decltype(&Base::Fun)>::no_op()
