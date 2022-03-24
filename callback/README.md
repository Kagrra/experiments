## callback
One day I needed to update our mocking code with bunch of callbacks. It was a boring task so I decided to use `std::function<void()>` for everything, attach empty lambdas to it and call it a day. Code was simple:

```c++
struct A {
    int read();
    void write(int a, int b);

    std::function<void()> on_read = []{};
    std::function<void()> on_write = []{};
};
```

During code review it was suggested that maybe using callback with signature identical to member function could be beneficial so I gave it a try.

```c++
struct A {
    int read();
    void write(int a, int b);

    std::function<int()> on_read = [](){return int{};};
    std::function<void(int, int)> on_write = [](int a, int b){};
};
```
This is a lot of work and is error prone, so surely there is a way to generate it automatically using a bit o metaprogramming and it is quite simple.

```c++
template<typename Signature>
struct callback;
 
template<typename R, typename... Args>
struct callback<R(Args...)> {
using type = std::function<R(Args...)>;
};

template<typename Base, typename R, typename... Args>
struct callback<R(Base::*)(Args...)>:callback<R(Args...)> {};
```

Sadly using it is not as pleasant (no_op implementation is in header file).
```c++
struct A{
    int read(){return 0;}
    void write(int a, int b){}

    callback<decltype(&A::write)>::type on_write = callback<decltype(&A::write)>::no_op();
    callback<decltype(&A::read)>::type on_read = callback<decltype(&A::read)>::no_op();
};
```

I really hate macros but this really seems like a perfect job for some preprocessor string concatenation.
```c++
#define CALL(Base, Fun) callback<decltype(&Base::Fun)>::type on_##Fun = callback<decltype(&Base::Fun)>::no_op()

struct A{
    int read(){return 0;}
    void write(int a, int b){}

    CALL(A, read);
    CALL(A, write);
};
```