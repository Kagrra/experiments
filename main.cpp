#include "generic_pipe/pipe.h"

constexpr int fun(int a)
{
    return a + 4;
}

constexpr int bun(int a)
{
    return a / 2;
}

int main()
{
    constexpr int t = 14;
    constexpr auto p = pipe::start {} | fun | bun |
        [&](int a) {
            return a + t;
        }
        | pipe::fork {
            [](int a) { return a; }, //
            [](int a) { return a + 1; }, //
            [](int a) { return a + 2; }, //
        }
        | [](int a, int b, int c) {
              return a + b + c;
          };
    return p(0);
}