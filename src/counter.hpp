#ifndef COUNTER_HPP
#define COUNTER_HPP

#include <array>
#include <cstdint>

namespace mahjong::waits_predictor::counter {
  namespace lh {
    struct All {
      std::array<uint64_t, 5u> a{};  // A状態の数
      std::array<uint64_t, 5u> b1{}; // B1状態の数
      std::array<uint64_t, 5u> b2{}; // B2状態の数
      std::array<uint64_t, 5u> c{};  // C状態の数
      std::array<uint64_t, 5u> d{};  // D状態の数
    };

    struct Each {
      std::array<std::array<uint64_t, 5u>, 9u> b2{}; // B2状態の数
      std::array<std::array<uint64_t, 5u>, 9u> c{};  // C状態の数
      std::array<std::array<uint64_t, 5u>, 9u> d{};  // D状態の数
    };

    void add1(All& lhs, const All& rhs, int m);
    uint64_t add2(const All& lhs, const All& rhs, int m);
    uint64_t add2(const All& lhs, const Each& rhs, int m, int i);
    All sum(const std::array<All, 4u>& all, const std::array<int, 3u>& suits, int m);
  }

  namespace sp {
    struct All {
      std::array<uint64_t, 7u> a{}; // A状態の数
      std::array<uint64_t, 7u> b{}; // B状態の数
    };

    struct Each {
      std::array<std::array<uint64_t, 7u>, 9u> b{}; // B状態の数
    };

    void add1(All& lhs, const All& rhs);
    uint64_t add2(const All& lhs, const All& rhs);
    uint64_t add2(const All& lhs, const Each& rhs, int i);
    All sum(const std::array<All, 4u>& all, const std::array<int, 3u>& suits);
  }
}

#endif
