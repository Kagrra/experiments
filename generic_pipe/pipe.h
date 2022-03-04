#ifndef PIPE_H
#define PIPE_H

#include <utility>

struct faucet {
  constexpr faucet() = default;

  template <typename T> constexpr auto operator()(T t) const { return t; }
};

template <typename L, typename R> class pipe {
public:
  constexpr pipe(L lhs, R rhs) : lhs_{lhs}, rhs_{rhs} {}

  template <typename... T> constexpr auto operator()(T &&...args) const {
    return call_rhs(lhs_(std::forward<T>(args)...));
  }

private:
  L lhs_;
  R rhs_;

  template <typename... T> constexpr auto call_rhs(T &&...args) const {
    return rhs_(std::forward<T>(args)...);
  }
};

template <typename L, typename R> constexpr auto operator|(L &&lhs, R &&rhs) {
  return pipe<L, R>(std::forward<L>(lhs), std::forward<R>(rhs));
}

#endif