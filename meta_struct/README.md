Based on great talk by [John Bandela](https://www.youtube.com/watch?v=FXfrojjIo80)

My motivation for this creation was very interesting ability to deduce partailly defined template types based on inheritance. For example

```c++
template<class T, class U>
struct S{};

template<typename ...T>
struct Base:: T... {}

using my_type = Base<
    S<int, int>,
    S<double, int>,
    S<float, double>
>;
```
For this to work we need to make sure first template parameter of `S` are unique and then we can do this:
```c++
template<typename S1, typename... T>
constexpr auto get(const Base<T...>& base) {
    auto cast = []<typename T, typename U>(named_type<N, U>& n) {
        return n;
    };
    return cast<S1>(base);
}

my_type b{};
get<double>(b); // return S<double, int>
```

So now `cast` lambda does all the work of searching for matching parameter.

## static_string

Is not very impressive, just holds chars in array and returns them as `string_view`. I've added two helper functions:
> ```c++
>template<size_t Lhs, size_t Rhs>
>constexpr auto operator+(const static_string<Lhs>& lhs, const static_string<Rhs>& rhs)
>```
Concatenation of two `static_string` objects.


>```c++
>template<size_t Value>
>constexpr auto itoa()
>```
Compile-time conversion from `size_t` to `static_string`

## named_type

It's just a type with a `static_string` name attached.

## meta_struct

This object is almost empty. For more interesting implementation check video above.

There are however few helper functions that are quite interesting:

>```c++
>template<static_string Name, typename... Members>
>constexpr auto& get(meta_struct<Members...>& m) noexcept
>```
Returns inherited type by name (by deduction trick from first paragraph).

>```c++
>template<typename Fun, typename... Members>
>constexpr decltype(auto) apply(Fun&& fun, counter<Members...>& c)
>```
Same as `std::apply` but instead of tuple it unrolls inehrited classes.

>```c++
>template<typename Fun, typename... Members>
>constexpr void for_each(Fun&& fun, meta_struct<Members...>& c)
>```
My favourite. Calls `fun` passing each inherited member as parameter.

>```c++
>template<static_string Name, typename T>
>struct get_type;
>```
Extracts type from `named_type` inherited by `T` 