#include "states.hpp"
#include "utils.hpp"
#include <cassert>
#include <fstream>
#include <functional>
#include <vector>
using namespace waits_predictor;
using namespace waits_predictor::states;
using Hand = std::vector<uint8_t>;
using Hands = std::vector<Hand>;

unsigned int generate(const int n,
                      const int m,
                      const int l,
                      Hand& hand,
                      std::function<bool(std::vector<uint8_t>&)> func)
{
  if (n >= static_cast<int>(hand.size())) {
    return func(hand) ? 1u : 0u;
  }

  unsigned int cnt = 0u;

  for (int i = std::max(m - l, 0); i <= std::min(m, 4); ++i) {
    hand[n] = i;
    cnt += generate(n + 1, m - i, l - 4, hand, func);
  }

  return cnt;
}

unsigned int generate(const int n,
                      const int m,
                      Hand& hand,
                      std::function<bool(std::vector<uint8_t>&)> func)
{
  return generate(n, m, 4 * (static_cast<int>(hand.size()) - 1), hand, func);
}

int main()
{
  {
    std::ofstream fout("lh_s_a.bin", std::ios_base::out | std::ios_base::binary);
    Hand hand(LEN_SUITS);

    for (unsigned int i = 0u; i < 5u; ++i) {
      const auto cnt = generate(0, 3 * i, hand, [&fout](Hand& hand) {
        if (iswh0(hand)) {
          fout.write(reinterpret_cast<const char*>(hand.data()), hand.size() * sizeof(Hand::value_type));

          return true;
        }
        else return false;
      });
      assert(cnt == lh::num_hands_a[IND_SUITS][i]);
    }
  }

  {
    std::ofstream fout("lh_h_a.bin", std::ios_base::out | std::ios_base::binary);
    Hand hand(LEN_HONOR);

    for (unsigned int i = 0u; i < 5u; ++i) {
      const auto cnt = generate(0, 3 * i, hand, [&fout](Hand& hand) {
        if (iswhh(hand)) {
          fout.write(reinterpret_cast<const char*>(hand.data()), hand.size() * sizeof(Hand::value_type));

          return true;
        }
        else return false;
      });
      assert(cnt == lh::num_hands_a[IND_HONOR][i]);
    }
  }

  {
    std::ofstream fout("lh_s_b.bin", std::ios_base::out | std::ios_base::binary);
    Hand hand(LEN_SUITS);

    for (unsigned int i = 0u; i < 4u; ++i) {
      const auto cnt = generate(0, 3 * i + 2, hand, [&fout](Hand& hand) {
        if (const auto wait = isrh(hand, iswh0); iswh2(hand)) {
          fout.write(reinterpret_cast<const char*>(hand.data()), hand.size() * sizeof(Hand::value_type));
          fout.write(reinterpret_cast<const char*>(&wait), sizeof(wait));

          return true;
        }
        else return false;
      });
      assert(cnt == lh::num_hands_b[IND_SUITS][i]);
    }
  }

  {
    std::ofstream fout("lh_h_b.bin", std::ios_base::out | std::ios_base::binary);
    Hand hand(LEN_HONOR);

    for (unsigned int i = 0u; i < 4u; ++i) {
      const auto cnt = generate(0, 3 * i + 2, hand, [&fout](Hand& hand) {
        if (const auto wait = isrh(hand, iswhh); iswhh(hand)) {
          fout.write(reinterpret_cast<const char*>(hand.data()), hand.size() * sizeof(Hand::value_type));
          fout.write(reinterpret_cast<const char*>(&wait), sizeof(wait));

          return true;
        }
        else return false;
      });
      assert(cnt == lh::num_hands_b[IND_HONOR][i]);
    }
  }

  {
    std::ofstream fout("lh_s_c.bin", std::ios_base::out | std::ios_base::binary);
    Hand hand(LEN_SUITS);

    for (unsigned int i = 0u; i < 4u; ++i) {
      const auto cnt = generate(0, 3 * i + 2, hand, [&fout](Hand& hand) {
        if (const auto wait = isrh(hand, iswh0); wait && !iswh2(hand)) {
          fout.write(reinterpret_cast<const char*>(hand.data()), hand.size() * sizeof(Hand::value_type));
          fout.write(reinterpret_cast<const char*>(&wait), sizeof(wait));

          return true;
        }
        else return false;
      });
      assert(cnt == lh::num_hands_c[IND_SUITS][i]);
    }
  }

  // NOTE: 字牌のC状態は存在しない

  {
    std::ofstream fout("lh_s_d.bin", std::ios_base::out | std::ios_base::binary);
    Hand hand(LEN_SUITS);

    for (unsigned int i = 0u; i < 5u; ++i) {
      const auto cnt = generate(0, 3 * i + 1, hand, [&fout](Hand& hand) {
        if (const auto wait = isrh(hand, iswh2); wait) {
          fout.write(reinterpret_cast<const char*>(hand.data()), hand.size() * sizeof(Hand::value_type));
          fout.write(reinterpret_cast<const char*>(&wait), sizeof(wait));

          return true;
        }
        else return false;
      });
      assert(cnt == lh::num_hands_d[IND_SUITS][i]);
    }
  }

  {
    std::ofstream fout("lh_h_d.bin", std::ios_base::out | std::ios_base::binary);
    Hand hand(LEN_HONOR);

    for (unsigned int i = 0u; i < 5u; ++i) {
      const auto cnt = generate(0, 3 * i + 1, hand, [&fout](Hand& hand) {
        if (const auto wait = isrh(hand, iswhh); wait) {
          fout.write(reinterpret_cast<const char*>(hand.data()), hand.size() * sizeof(Hand::value_type));
          fout.write(reinterpret_cast<const char*>(&wait), sizeof(wait));

          return true;
        }
        else return false;
      });
      assert(cnt == lh::num_hands_d[IND_HONOR][i]);
    }
  }

  {
    std::ofstream fout("sp_s_a.bin", std::ios_base::out | std::ios_base::binary);
    Hand hand(LEN_SUITS);

    for (unsigned int i = 0u; i < 7u; ++i) {
      const auto cnt = generate(0, 2 * i, hand, [&fout](Hand& hand) {
        if (issp(hand)) {
          fout.write(reinterpret_cast<const char*>(hand.data()), hand.size() * sizeof(Hand::value_type));

          return true;
        }
        else return false;
      });
      assert(cnt == sp::num_hands_a[IND_SUITS][i]);
    }
  }

  {
    std::ofstream fout("sp_h_a.bin", std::ios_base::out | std::ios_base::binary);
    Hand hand(LEN_HONOR);

    for (unsigned int i = 0u; i < 7u; ++i) {
      const auto cnt = generate(0, 2 * i, hand, [&fout](Hand& hand) {
        if (issp(hand)) {
          fout.write(reinterpret_cast<const char*>(hand.data()), hand.size() * sizeof(Hand::value_type));

          return true;
        }
        else return false;
      });
      assert(cnt == sp::num_hands_a[IND_HONOR][i]);
    }
  }

  {
    std::ofstream fout("sp_s_b.bin", std::ios_base::out | std::ios_base::binary);
    Hand hand(LEN_SUITS);

    for (unsigned int i = 0u; i < 7u; ++i) {
      const auto cnt = generate(0, 2 * i + 1, hand, [&fout](Hand& hand) {
        if (const auto wait = isrh(hand, issp); wait) {
          fout.write(reinterpret_cast<const char*>(hand.data()), hand.size() * sizeof(Hand::value_type));
          fout.write(reinterpret_cast<const char*>(&wait), sizeof(wait));

          return true;
        }
        else return false;
      });
      assert(cnt == sp::num_hands_b[IND_SUITS][i]);
    }
  }

  {
    std::ofstream fout("sp_h_b.bin", std::ios_base::out | std::ios_base::binary);
    Hand hand(LEN_HONOR);

    for (unsigned int i = 0u; i < 7u; ++i) {
      const auto cnt = generate(0, 2 * i + 1, hand, [&fout](Hand& hand) {
        if (const auto wait = isrh(hand, issp); wait) {
          fout.write(reinterpret_cast<const char*>(hand.data()), hand.size() * sizeof(Hand::value_type));
          fout.write(reinterpret_cast<const char*>(&wait), sizeof(wait));

          return true;
        }
        else return false;
      });
      assert(cnt == sp::num_hands_b[IND_HONOR][i]);
    }
  }

  {
    std::ofstream fout("lh_sp_s_a.bin", std::ios_base::out | std::ios_base::binary);
    Hand hand(LEN_SUITS);

    for (unsigned int i = 0u; i < 5u; ++i) {
      const auto cnt = generate(0, 3 * i, hand, [&fout](Hand& hand) {
        if (iswh0(hand) && issp(hand)) {
          fout.write(reinterpret_cast<const char*>(hand.data()), hand.size() * sizeof(Hand::value_type));

          return true;
        }
        else return false;
      });
      assert(cnt == lh_sp::num_hands_a[IND_SUITS][i]);
    }
  }

  {
    std::ofstream fout("lh_sp_h_a.bin", std::ios_base::out | std::ios_base::binary);
    Hand hand(LEN_HONOR);

    for (unsigned int i = 0u; i < 5u; ++i) {
      const auto cnt = generate(0, 3 * i, hand, [&fout](Hand& hand) {
        if (iswhh(hand) && issp(hand)) {
          fout.write(reinterpret_cast<const char*>(hand.data()), hand.size() * sizeof(Hand::value_type));

          return true;
        }
        else return false;
      });
      assert(cnt == lh_sp::num_hands_a[IND_HONOR][i]);
    }
  }

  {
    std::ofstream fout("lh_sp_s_b.bin", std::ios_base::out | std::ios_base::binary);
    Hand hand(LEN_SUITS);

    for (unsigned int i = 0u; i < 4u; ++i) {
      const auto cnt = generate(0, 3 * i + 2, hand, [&fout](Hand& hand) {
        if (iswh2(hand) && issp(hand)) {
          fout.write(reinterpret_cast<const char*>(hand.data()), hand.size() * sizeof(Hand::value_type));

          return true;
        }
        else return false;
      });
      assert(cnt == lh_sp::num_hands_b[IND_SUITS][i]);
    }
  }

  {
    std::ofstream fout("lh_sp_h_b.bin", std::ios_base::out | std::ios_base::binary);
    Hand hand(LEN_HONOR);

    for (unsigned int i = 0u; i < 4u; ++i) {
      const auto cnt = generate(0, 3 * i + 2, hand, [&fout](Hand& hand) {
        if (iswhh(hand) && issp(hand)) {
          fout.write(reinterpret_cast<const char*>(hand.data()), hand.size() * sizeof(Hand::value_type));

          return true;
        }
        else return false;
      });
      assert(cnt == lh_sp::num_hands_b[IND_HONOR][i]);
    }
  }

  {
    std::ofstream fout("lh_sp_s_c.bin", std::ios_base::out | std::ios_base::binary);
    Hand hand(LEN_SUITS);

    for (unsigned int i = 0u; i < 4u; ++i) {
      const auto cnt = generate(0, 3 * i + 2, hand, [&fout](Hand& hand) {
        if (const auto wait = isrh(hand, iswh0); wait && isrh(hand, issp)) {
          fout.write(reinterpret_cast<const char*>(hand.data()), hand.size() * sizeof(Hand::value_type));
          fout.write(reinterpret_cast<const char*>(&wait), sizeof(wait));

          return true;
        }
        else return false;
      });
      assert(cnt == lh_sp::num_hands_c[IND_SUITS][i]);
    }
  }

  // NOTE: 字牌のC状態は存在しない

  {
    std::ofstream fout("lh_sp_s_d.bin", std::ios_base::out | std::ios_base::binary);
    Hand hand(LEN_SUITS);

    for (unsigned int i = 0u; i < 5u; ++i) {
      const auto cnt = generate(0, 3 * i + 1, hand, [&fout](Hand& hand) {
        if (const auto wait = isrh(hand, iswh2); wait && isrh(hand, issp)) {
          fout.write(reinterpret_cast<const char*>(hand.data()), hand.size() * sizeof(Hand::value_type));
          fout.write(reinterpret_cast<const char*>(&wait), sizeof(wait));

          return true;
        }
        else return false;
      });
      assert(cnt == lh_sp::num_hands_d[IND_SUITS][i]);
    }
  }

  {
    std::ofstream fout("lh_sp_h_d.bin", std::ios_base::out | std::ios_base::binary);
    Hand hand(LEN_HONOR);

    for (unsigned int i = 0u; i < 5u; ++i) {
      const auto cnt = generate(0, 3 * i + 1, hand, [&fout](Hand& hand) {
        if (const auto wait = isrh(hand, iswhh); wait && isrh(hand, issp)) {
          fout.write(reinterpret_cast<const char*>(hand.data()), hand.size() * sizeof(Hand::value_type));
          fout.write(reinterpret_cast<const char*>(&wait), sizeof(wait));

          return true;
        }
        else return false;
      });
      assert(cnt == lh_sp::num_hands_d[IND_HONOR][i]);
    }
  }

  return EXIT_SUCCESS;
}
