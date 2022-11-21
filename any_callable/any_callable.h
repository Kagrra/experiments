#ifndef KGR_ANY_CALLABLE_H
#define KGR_ANY_CALLABLE_H

#include <array>
#include <functional>

template<class T, std::size_t Size> class any_callable;

namespace details
{
template<class Fun, class Ret, class... Args> concept invocable = requires(Fun&& fun, Args&&... args)
{
    {
        std::invoke(std::forward<Fun>(fun), std::forward<Args>(args)...)
    }
    ->std::convertible_to<Ret>;
};
} // namespace details
template<class Ret, class... Args, std::size_t Size> class any_callable<Ret(Args...), Size>
{
public:
    constexpr any_callable() = default;

    // copy constructor
    template<typename AnyCallable>
    requires(std::same_as<AnyCallable, any_callable>) constexpr any_callable(AnyCallable&& any) noexcept
    {
        this->ptr_ = any.ptr;
        this->buffer_ = any.buffer_;
    }

    // perfect forwarding constructor
    // requirements:
    // guard against shadowing copy constructor
    // checks if Fun is not a function
    // if Fun will fit in buffer
    // if signature matches any_callable
    template<typename Fun>
        requires(!std::same_as<Fun, any_callable>) && (!std::is_function_v<Fun>)&&(sizeof(Fun) <= Size)
        && details::invocable<Fun, Ret, Args...> any_callable(Fun&& fun)
    {
        new (buffer_.data()) Fun {std::forward<Fun>(fun)};
        ptr_ = [](std::byte* buf, Args... args) {
            (*reinterpret_cast<Fun*>(buf))(std::forward<Args>(args)...);
        };
    }

    // function pointer constructor
    // requirements:
    // if Fun is function
    // if signature matches any_callable
    template<typename Fun>
        requires(std::is_function_v<Fun>) && details::invocable<Fun, Ret, Args...> any_callable(Fun&& fun)
    {
        new (buffer_.data()) Fun {std::forward<Fun>(fun)};
        ptr_ = [](std::byte* buf, Args... args) {
            reinterpret_cast<Fun*>(buf)(std::forward<Args>(args)...);
        };
    }

    // assignment operator
    template<typename AnyCallable>
    requires(std::same_as<AnyCallable, any_callable>) constexpr any_callable& operator=(AnyCallable&& any)
    {
        this->ptr_ = any.ptr;
        this->buffer_ = any.buffer_;
        return *this;
    }

    template<typename Fun>
        requires(!std::same_as<Fun, any_callable>) && (!std::is_function_v<std::decay<Fun>>)&&(sizeof(Fun) <= Size)
        && details::invocable<Fun, Ret, Args...> any_callable& operator=(Fun&& fun)
    {
        new (buffer_.data()) Fun {std::forward<Fun>(fun)};
        ptr_ = [](std::byte* buf, Args... args) {
            (*reinterpret_cast<Fun*>(buf))(std::forward<Args>(args)...);
        };
        return *this;
    }

    template<typename R, typename... Ts>
    requires details::invocable<R (*)(Ts...), Ret, Args...> any_callable& operator=(R (*fun)(Ts...))
    {
        using function_type = R (*)(Ts...);
        new (buffer_.data()) function_type {fun};
        ptr_ = [](std::byte* buf, Args... args) {
            (*reinterpret_cast<function_type*>(buf))(std::forward<Args>(args)...);
        };
        return *this;
    }

    template<typename AnyCallable>
    requires(std::is_same_v<any_callable, AnyCallable>) constexpr any_callable& operator=(AnyCallable&& any) noexcept
    {
        this->ptr_ = any.ptr;
        this->buffer_ = any.buffer_;
        return *this;
    }

    template<typename... Ts> void operator()(Ts... arg)
    {
        return ptr_(buffer_.data(), std::forward<Ts>(arg)...);
    }

    static constexpr Ret noop(std::byte*, Args...) noexcept
    {
        if constexpr (!std::same_as<Ret, void>)
        {
            return Ret {};
        }
    }

private:
    void (*ptr_)(std::byte*, Args...) {any_callable::noop};
    alignas(Ret (*)(Args...)) std::array<std::byte, Size> buffer_;
};

#endif