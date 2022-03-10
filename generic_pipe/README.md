## pipe
After seeing pipes on ranges in C++20 I wanted to try doing something similar, but with functions. I imagined it would be nice to chain them together in more readable way. It could be used like `receive | parse | response | send` where each function pipes return value to the next one.

As I like idea of `constexpr` everything I imagined compiler would nicely inline and optimize each `constexpr` function call (as `constexpr` implies `inline`). It works up to a point.

Based on one of great talks by [Michael Caisse](https://www.youtube.com/watch?v=OkeRijjmoh8)

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