#ifndef COUNTER_HPP
#define COUNTER_HPP

#include <array>
#include <cstdint>

namespace waits_predictor::counter {
  namespace lh {
    struct All {
      std::array<uint64_t, 5u> a{};
      std::array<uint64_t, 5u> b1{};
      std::array<uint64_t, 5u> b2{};
      std::array<uint64_t, 5u> c{};
      std::array<uint64_t, 5u> d{};
    };

    struct Each {
      std::array<std::array<uint64_t, 5u>, 9u> b2{};
      std::array<std::array<uint64_t, 5u>, 9u> c{};
      std::array<std::array<uint64_t, 5u>, 9u> d{};
    };

    void add1(All& lhs, const All& rhs, int m);
    uint64_t add2(const All& lhs, const All& rhs, int m);
    uint64_t add2(const All& lhs, const Each& rhs, int m, int i);
    All sum(const std::array<All, 4u>& all, const std::array<int, 3u>& suits, int m);
  }

  namespace sp {
    struct All {
      std::array<uint64_t, 7u> a{};
      std::array<uint64_t, 7u> b{};
    };

    struct Each {
      std::array<std::array<uint64_t, 7u>, 9u> b{};
    };

    void add1(All& lhs, const All& rhs);
    uint64_t add2(const All& lhs, const All& rhs);
    uint64_t add2(const All& lhs, const Each& rhs, int i);
    All sum(const std::array<All, 4u>& all, const std::array<int, 3u>& suits);
  }
}

#endif
