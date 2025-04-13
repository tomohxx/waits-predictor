#include "waits_predictor.hpp"
#include "counter.hpp"
#include <array>
#include <cassert>
#include <format>
#include <numeric>
#include <stdexcept>
#include <tuple>
#include <type_traits>

namespace {
  // combination
  constexpr std::array<std::array<unsigned int, 5u>, 5u> combin = {{
      {{1u, 0u, 0u, 0u, 0u}}, // 0Cr
      {{1u, 1u, 0u, 0u, 0u}}, // 1Cr
      {{1u, 2u, 1u, 0u, 0u}}, // 2Cr
      {{1u, 3u, 3u, 1u, 0u}}, // 3Cr
      {{1u, 4u, 6u, 4u, 1u}}, // 4Cr
  }};
}

namespace waits_predictor {
  namespace internal {
    constexpr std::array<int, 13u> tile_ids = {0, 8, 9, 17, 18, 26, 27, 28, 29, 30, 31, 32, 33};
    constexpr std::array<std::array<int, 3u>, 4u> suits = {{
        {{1, 2, 3}},
        {{0, 2, 3}},
        {{0, 1, 3}},
        {{0, 1, 2}},
    }};

    using All = decltype(counter::lh::All::a);
    using Each = decltype(counter::lh::Each::b);

    static_assert(std::is_same<All, decltype(counter::sp::All::a)>::value);
    static_assert(std::is_same<Each, decltype(counter::sp::Each::b)>::value);

    Value calc_lh(const std::vector<counter::lh::All>& all, const std::vector<counter::lh::Each>& each, int m);
    Value calc_sp(const std::vector<counter::sp::All>& all, const std::vector<counter::sp::Each>& each);
  }

  Value calc_lh(const Hand& wall, unsigned int river, int m);
  Value calc_sp(const Hand& wall, unsigned int river);
  Value calc_lh_sp(const Hand& wall, unsigned int river);
  Value calc_to(const std::vector<int>& wall, const std::vector<int>& river);

  std::array<Value, 4u> predict_waits(const std::vector<int>& wall,
                                      const std::vector<int>& river,
                                      const int m,
                                      const bool check)
  {
    if (check) {
      if (const auto size = wall.size(); size != NUM_TIDS) {
        throw std::invalid_argument(std::format("Invalid size of wall: {}", size));
      }

      if (const auto size = river.size(); size != NUM_TIDS) {
        throw std::invalid_argument(std::format("Invalid size of river: {}", size));
      }

      for (unsigned int i = 0u; i < NUM_TIDS; ++i) {
        if (wall[i] < 0 || wall[i] > 4) {
          throw std::invalid_argument(std::format("Invalid number of wall's tiles at {}: {}", i, wall[i]));
        }
      }

      for (unsigned int i = 0u; i < NUM_TIDS; ++i) {
        if (river[i] < 0 || river[i] > 4) {
          throw std::invalid_argument(std::format("Invalid number of river's tiles at {}: {}", i, river[i]));
        }
      }

      if (m < 0 || m > 4) {
        throw std::invalid_argument(std::format("Invalid sum of hands's melds: {}", m));
      }
    }

    const Hand wall_(wall.begin(), wall.end());
    const unsigned int river_ = std::accumulate(river.crbegin(),
                                                river.crend(),
                                                0u,
                                                [](int acc, const int& x) { return (acc << 1) | !!x; });

    const auto ret_lh = calc_lh(wall_, river_, m);
    auto ret_sp = (m == 4 ? calc_sp(wall_, river_) : Value{});
    const auto ret_lh_sp = (m == 4 ? calc_lh_sp(wall_, river_) : Value{});
    const auto ret_to = (m == 4 ? calc_to(wall, river) : Value{});

    ret_sp -= ret_lh_sp;

    return {ret_lh + ret_sp + ret_to, ret_lh, ret_sp, ret_to};
  }

  unsigned int count_combin(const Hand& wall, const Hand& hand)
  {
    unsigned int ret = 1u;

    for (std::size_t i = 0; i < hand.size(); ++i) {
      ret *= combin[wall[i]][hand[i]];
    }

    return ret;
  }

  internal::All count_all(const std::vector<Hands>& hands, const Hand& wall)
  {
    assert(hands[0][0].size() == wall.size());

    internal::All all(hands.size());

    for (std::size_t i = 0u; i < hands.size(); ++i) {
      for (std::size_t j = 0u; j < hands[i].size(); ++j) {
        all[i] += count_combin(wall, hands[i][j]);
      }
    }

    return all;
  }

  std::tuple<internal::All, internal::Each> count_each(const std::vector<HandWaits>& hand_waits,
                                                       const Hand& wall,
                                                       const uint16_t river)
  {
    // assert(hand_waits[0][0].hand.size() == wall.size());

    internal::All all(hand_waits.size());
    internal::Each each(wall.size(), internal::Each::value_type(hand_waits.size()));

    for (std::size_t i = 0u; i < hand_waits.size(); ++i) {
      for (std::size_t j = 0u; j < hand_waits[i].size(); ++j) {
        const auto tmp = count_combin(wall, hand_waits[i][j].hand);
        const auto wait = hand_waits[i][j].wait;

        if ((wait & (~river)) != wait) continue;

        all[i] += tmp;

        for (std::size_t k = 0u; k < wall.size(); ++k) {
          each[k][i] += (wait & (1u << k)) ? tmp : 0u;
        }
      }
    }

    return {all, each};
  }

  Value calc_lh(const Hand& wall, const unsigned int river, const int m)
  {
    std::vector<counter::lh::All> all(4u);
    std::vector<counter::lh::Each> each(4u);

    for (std::size_t i = 0u; i < 3u; ++i) {
      const Hand wall_(wall.cbegin() + 9 * i, wall.cbegin() + 9 * i + 9);
      const uint16_t river_ = (river >> (9 * i)) & 0x1FF;

      all[i].a = count_all(states::lh::hands_a[0], wall_);
      std::tie(all[i].b, each[i].b) = count_each(states::lh::hands_b[IND_SUITS], wall_, river_);
      std::tie(all[i].c, each[i].c) = count_each(states::lh::hands_c[IND_SUITS], wall_, river_);
      std::tie(all[i].d, each[i].d) = count_each(states::lh::hands_d[IND_SUITS], wall_, river_);
    }

    {
      const Hand wall_(wall.cbegin() + 27, wall.cend());
      const uint16_t river_ = (river >> 27) & 0x7F;

      all[3].a = count_all(states::lh::hands_a[1], wall_);
      std::tie(all[3].b, each[3].b) = count_each(states::lh::hands_b[IND_HONOR], wall_, river_);
      std::tie(all[3].d, each[3].d) = count_each(states::lh::hands_d[IND_HONOR], wall_, river_);
    }

    return internal::calc_lh(all, each, m);
  }

  Value calc_sp(const Hand& wall, const unsigned int river)
  {
    std::vector<counter::sp::All> all(4u);
    std::vector<counter::sp::Each> each(4u);

    for (std::size_t i = 0u; i < 3u; ++i) {
      const Hand wall_(wall.cbegin() + 9 * i, wall.cbegin() + 9 * i + 9);
      const uint16_t river_ = (river >> (9 * i)) & 0x1FF;

      all[i].a = count_all(states::sp::hands_a[0], wall_);
      std::tie(all[i].b, each[i].b) = count_each(states::sp::hands_b[IND_SUITS], wall_, river_);
    }

    {
      const Hand wall_(wall.cbegin() + 27, wall.cend());
      const uint16_t river_ = (river >> 27) & 0x7F;

      all[3].a = count_all(states::sp::hands_a[1], wall_);
      std::tie(all[3].b, each[3].b) = count_each(states::sp::hands_b[IND_HONOR], wall_, river_);
    }

    return internal::calc_sp(all, each);
  }

  Value calc_lh_sp(const Hand& wall, const unsigned int river)
  {
    std::vector<counter::lh::All> all(4u);
    std::vector<counter::lh::Each> each(4u);

    for (std::size_t i = 0u; i < 3u; ++i) {
      const Hand wall_(wall.cbegin() + 9 * i, wall.cbegin() + 9 * i + 9);
      const uint16_t river_ = (river >> (9 * i)) & 0x1FF;

      all[i].a = count_all(states::lh_sp::hands_a[0], wall_);
      all[i].b = count_all(states::lh_sp::hands_b[0], wall_);
      std::tie(all[i].c, each[i].c) = count_each(states::lh_sp::hands_c[IND_SUITS], wall_, river_);
      std::tie(all[i].d, each[i].d) = count_each(states::lh_sp::hands_d[IND_SUITS], wall_, river_);
    }

    {
      const Hand wall_(wall.cbegin() + 27, wall.cend());
      const uint16_t river_ = (river >> 27) & 0x7F;

      all[3].a = count_all(states::lh_sp::hands_a[IND_HONOR], wall_);
      all[3].b = count_all(states::lh_sp::hands_b[IND_HONOR], wall_);
      std::tie(all[3].d, each[3].d) = count_each(states::lh_sp::hands_d[IND_HONOR], wall_, river_);
    }

    return internal::calc_lh(all, each, 4);
  }

  Value calc_to(const std::vector<int>& wall, const std::vector<int>& river)
  {
    using internal::tile_ids;

    Value ret;

    // 国士無双一面待ち

    for (const auto& i : tile_ids) {   // 雀頭
      for (const auto& j : tile_ids) { // 待ち
        if (i == j) continue;

        unsigned int tmp = 1u;

        for (const auto& k : tile_ids) {
          const int r = (k == i ? 2 : (k == j ? 0 : 1));

          tmp *= combin[wall[i]][r];
        }

        if (!river[j]) {
          ret.all += tmp;
          ret.each[j] += tmp;
        }
      }
    }

    for (const auto& i : tile_ids) {
      if (river[i]) return ret;
    }

    // 国士無双十三面待ち

    {
      unsigned int tmp = 1u;

      for (const auto& i : tile_ids) {
        tmp *= combin[wall[i]][1];
      }

      ret.all += tmp;

      for (const auto& i : tile_ids) {
        ret.each[i] += tmp;
      }
    }

    return ret;
  }

  namespace internal {
    Value calc_lh(const std::vector<counter::lh::All>& all, const std::vector<counter::lh::Each>& each, const int m)
    {
      assert(all.size() == 4u);
      assert(each.size() == 4u);

      Value ret;

      ret.all = add2(sum(all, suits[3], m), all[3], m);

      for (std::size_t i = 0u; i < 3u; ++i) {
        const auto tmp = sum(all, suits[i], m);

        for (std::size_t j = 0u; j < 9u; ++j) {
          ret.each[9 * i + j] = add2(tmp, each[i], m, j);
        }
      }

      {
        const auto tmp = sum(all, suits[3], m);

        for (std::size_t j = 0u; j < 7u; ++j) {
          ret.each[27 + j] = add2(tmp, each[3], m, j);
        }
      }

      return ret;
    }

    Value calc_sp(const std::vector<counter::sp::All>& all, const std::vector<counter::sp::Each>& each)
    {
      assert(all.size() == 4u);
      assert(each.size() == 4u);

      Value ret;

      ret.all = add2(sum(all, suits[3]), all[3]);

      for (std::size_t i = 0u; i < 3u; ++i) {
        const auto tmp = sum(all, suits[i]);

        for (std::size_t j = 0u; j < 9u; ++j) {
          ret.each[9 * i + j] = add2(tmp, each[i], j);
        }
      }

      {
        const auto tmp = sum(all, suits[3]);

        for (std::size_t j = 0u; j < 7u; ++j) {
          ret.each[27 + j] = add2(tmp, each[3], j);
        }
      }

      return ret;
    }
  }
}
