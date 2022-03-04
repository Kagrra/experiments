#include "generic_pipe/pipe.h"

constexpr int fun(int a) { return a + 4; }

constexpr int bun(int a) { return a / 2; }

int main() {
  constexpr int t = 14;
  constexpr auto p = faucet{} | fun | bun | [&](int a) { return a + t; };

  return p(4);
}