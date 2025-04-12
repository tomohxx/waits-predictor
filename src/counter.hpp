#ifndef COUNTER_HPP
#define COUNTER_HPP

#include <cstdint>
#include <utility>
#include <vector>

namespace waits_predictor::counter {
  namespace lh {
    struct All {
      std::vector<uint64_t> a;
      std::vector<uint64_t> b;
      std::vector<uint64_t> c;
      std::vector<uint64_t> d;

      All() : a(5u), b(5u), c(5u), d(5u) {}
    };

    struct Each {
      std::vector<std::vector<uint64_t>> b;
      std::vector<std::vector<uint64_t>> c;
      std::vector<std::vector<uint64_t>> d;

      Each() : b(9u, std::vector<uint64_t>(5u)),
               c(9u, std::vector<uint64_t>(5u)),
               d(9u, std::vector<uint64_t>(5u)) {}
    };

    void add1(All& lhs, const All& rhs, int m);
    uint64_t add2(const All& lhs, const All& rhs, int m);
    uint64_t add2(const All& lhs, const Each& rhs, int m, int i);
    All sum(const std::vector<All>& all, const std::array<int, 3u>& suits, int m);
  }

  namespace sp {
    struct All {
      std::vector<uint64_t> a;
      std::vector<uint64_t> b;

      All() : a(7u), b(7u) {}
    };

    struct Each {
      std::vector<std::vector<uint64_t>> b;

      Each() : b(9u, std::vector<uint64_t>(7u)) {}
    };

    void add1(All& lhs, const All& rhs);
    uint64_t add2(const All& lhs, const All& rhs);
    uint64_t add2(const All& lhs, const Each& rhs, int i);
    All sum(const std::vector<All>& all, const std::array<int, 3u>& suits);
  }
}

#endif
