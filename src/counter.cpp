#include "counter.hpp"
#include <array>
#include <cassert>

namespace waits_predictor::counter {
  namespace lh {
    void add1(All& lhs, const All& rhs, const int m)
    {
      for (int j = m; j >= 0; --j) {
        uint64_t tmp = 0u;

        for (int k = 0; k <= j; ++k) {
          tmp += lhs.a[k] * rhs.d[j - k] + lhs.d[k] * rhs.a[j - k];
        }

        for (int k = 0; k <= j - 1; ++k) {
          tmp += lhs.b[k] * rhs.c[j - k - 1] +
                 lhs.c[k] * rhs.b[j - k - 1] +
                 lhs.b[k] * rhs.b[j - k - 1];
        }

        lhs.d[j] = tmp;
      }

      for (int j = m - 1; j >= 0; --j) {
        uint64_t tmp = 0u;

        for (int k = 0; k <= j; ++k) {
          tmp += lhs.a[k] * rhs.c[j - k] + lhs.c[k] * rhs.a[j - k];
        }

        lhs.c[j] = tmp;
      }

      for (int j = m - 1; j >= 0; --j) {
        uint64_t tmp = 0u;

        for (int k = 0; k <= j; ++k) {
          tmp += lhs.a[k] * rhs.b[j - k] + lhs.b[k] * rhs.a[j - k];
        }

        lhs.b[j] = tmp;
      }

      for (int j = m; j >= 0; --j) {
        uint64_t tmp = 0u;

        for (int k = 0; k <= j; ++k) {
          tmp += lhs.a[k] * rhs.a[j - k];
        }

        lhs.a[j] = tmp;
      }
    }

    uint64_t add2(const All& lhs, const All& rhs, const int m)
    {
      uint64_t ret = 0u;

      for (int k = 0; k <= m; ++k) {
        ret += lhs.a[k] * rhs.d[m - k] + lhs.d[k] * rhs.a[m - k];
      }

      for (int k = 0; k <= m - 1; ++k) {
        ret += lhs.b[k] * rhs.c[m - k - 1] +
               lhs.c[k] * rhs.b[m - k - 1] +
               lhs.b[k] * rhs.b[m - k - 1];
      }

      return ret;
    }

    uint64_t add2(const All& lhs, const Each& rhs, const int m, const int i)
    {
      uint64_t ret = 0u;

      for (int k = 0; k <= m; ++k) {
        ret += lhs.a[k] * rhs.d[i][m - k];
      }

      for (int k = 0; k <= m - 1; ++k) {
        ret += lhs.b[k] * rhs.c[i][m - k - 1] + lhs.b[k] * rhs.b[i][m - k - 1];
      }

      return ret;
    }

    All sum(const std::vector<All>& all, const std::array<int, 3u>& suits, const int m)
    {
      assert(suits.size() > 0u);

      All ret = all[suits[0]];

      for (std::size_t i = 1u; i < suits.size(); ++i) {
        add1(ret, all[suits[i]], m);
      }

      return ret;
    }
  }

  namespace sp {
    void add1(All& lhs, const All& rhs)
    {
      for (int j = 6; j >= 0; --j) {
        uint64_t tmp = 0u;

        for (int k = 0; k <= j; ++k) {
          tmp += lhs.a[k] * rhs.b[j - k] + lhs.b[k] * rhs.a[j - k];
        }

        lhs.b[j] = tmp;
      }

      for (int j = 6; j >= 0; --j) {
        uint64_t tmp = 0u;

        for (int k = 0; k <= j; ++k) {
          tmp += lhs.a[k] * rhs.a[j - k];
        }

        lhs.a[j] = tmp;
      }
    }

    uint64_t add2(const All& lhs, const All& rhs)
    {
      uint64_t ret = 0u;

      for (int k = 0; k <= 6; ++k) {
        ret += lhs.a[k] * rhs.b[6 - k] + lhs.b[k] * rhs.a[6 - k];
      }

      return ret;
    }

    uint64_t add2(const All& lhs, const Each& rhs, const int i)
    {
      uint64_t ret = 0u;

      for (int k = 0; k <= 6; ++k) {
        ret += lhs.a[k] * rhs.b[i][6 - k];
      }

      return ret;
    }

    All sum(const std::vector<All>& all, const std::array<int, 3u>& suits)
    {
      assert(suits.size() > 0u);

      All ret = all[suits[0]];

      for (std::size_t i = 1u; i < suits.size(); ++i) {
        add1(ret, all[suits[i]]);
      }

      return ret;
    }
  }
}
