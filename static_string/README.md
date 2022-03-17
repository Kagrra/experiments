Based on great talk by [John Bandela](https://www.youtube.com/watch?v=FXfrojjIo80)

## static_string
Is not very impressive as I struggle to find real world scenario where passing string as template parameter would be useful, but it allows some kind of manual reflection or static like storage of string_view.

What really interested me is ability to deduce partialy defined template types based on inheritance.

I've got system where I need to count number of different things happening. Sometime it's taking one path of code, other time it's capturing certain exception. While it would be easy to just use struct
```c++
struct count_me {
    int event_a {};
    int path_b {};
    int type_d {};
};

count_me counter;

counter.event_a++;
```
it's not fun. So instead one can use `counter` class like this:
```c++
counter <
    named_counter<"event_a", event_type>,
    named_counter<"path_b", int>,
    named_counter<"type_d", type_d>> c{};

c.inc<event_type>(); // this workd as long as names are uniqe
c.inc<int>(); // this works as long as types in named counter are unique
```
and so far it's my only idea to actually use it in some shape or form.

## get_type<static_string Name, typenem T>()

My friend challanged me to write function returning `type` part of the `named_type` from any container using inheritance trick. I came up with this solution:
```c++
namespace details
{
template<static_string Name, typename T> auto get_type_impl()
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

template<static_string Name, typename T> using get_type = decltype(details::get_type_impl<Name, T>())::type;
```
