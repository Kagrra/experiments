Based on one of great talks by [Michael Caisse](https://www.youtube.com/watch?v=OkeRijjmoh8)

## pipe
After seeing pipes on ranges in C++20 I wanted to try doing something similar, but with functions. I imagined it would be nice to chain them together in more readable way. It could be used like `receive | parse | response | send` where each function pipes return value to the next one.

As I like idea of `constexpr` everything I imagined compiler would nicely inline and optimize each `constexpr` function call (as `constexpr` implies `inline`). It works up to a point.

```c++
constexpr int fun(int a){return a+4;}
constexpr int bun(int a){return a/2;}
constexpr int gun(int a){return a-7;}

int aaa(int a)
{
    constexpr auto pp = faucet{} | fun | bun | gun;
    return pp(a);
}

int bbb(int a)
{
    return gun(bun(fun(a)));
}
```
this code works as expected. Both `aaa` and `bbb` are inlined and optimized with compiled code looking same on both clang and gcc (with `-std=c++20 -O3`):
```c
aaa(int):
        add     edi, 4
        mov     eax, edi
        shr     eax, 31
        add     eax, edi
        sar     eax
        sub     eax, 7
        ret
bbb(int):
        add     edi, 4
        mov     eax, edi
        shr     eax, 31
        add     eax, edi
        sar     eax
        sub     eax, 7
        ret
```

### pipe::start

Dummy object that only purpose is inject `pipe` namespace to `|` operator. Forwards agrument to the rest of the pipe.
```c++
constexpr auto calc = pipe::start | do_work | do_more_work;

static_assert(pipe::start{}(5) == 5);
```

### pipe::constant

Object holding constant value and passing it to next function in pipe. Does not expect to get any argument;
```c++
constexpr auto calc = pipe::constant{5} | do_work; //

static_assert(calc() == do_work(5));
```

### pipe::fork

Class holding bunch of function-like objects. Forwards each result as tuple or by `std::apply`
```c++
auto fun1 = [](int a){return a+1;};
auto fun2 = [](int a){return a+2;};
auto fun3 = [](int a){return a+3;};

auto final_fun = [](int a, int b, int c){return a+b+c;};
auto final_fun_2 = [](std::tuple<int,int,int> a){ return std::get<0>(a) + std::get<1>(a) + std::get<2>(a);}
// std::tuple also works as function argument after fork

constexpr auto calc = pipe::start | pipe::fork {fun1,fun2,fun3} | final_fun;
// constexpr auto calc = pipe::start | pipe::fork {fun1,fun2,fun3} | final_fun_2; // same as above by with tuple
```
`calc(2);` is same as calling `final_fun(fun1(2), fun2(2), fun3(2));`



<details>
<summary>Update - below is fixed. gcc still generates functions body but inlines pipes call.</summary>

Sadly chaining more functions breaks on gcc (still works as expected on clang). Test after adding one more function call:

```c++
int aaa(int a)
{
    constexpr auto pp = faucet{} | fun | bun | gun | fun;
    return pp(a);
}

int bbb(int a)
{
    return fun(gun(bun(fun(a))));
}
```
Generates assembler code with function bodies and calls :(
```c
fun(int):
        lea     eax, [rdi+4]
        ret
bun(int):
        mov     eax, edi
        shr     eax, 31
        add     eax, edi
        sar     eax
        ret
gun(int):
        lea     eax, [rdi-7]
        ret
aaa(int):
        sub     rsp, 8
        add     edi, 4
        call    bun(int)
        mov     edi, eax
        call    gun(int)
        add     rsp, 8
        mov     edi, eax
        jmp     fun(int)
bbb(int):
        add     edi, 4
        mov     eax, edi
        shr     eax, 31
        add     eax, edi
        sar     eax
        sub     eax, 3
        ret
```
</details>