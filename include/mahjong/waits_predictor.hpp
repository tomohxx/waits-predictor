#ifndef MAHJONG_WAITS_PREDICTOR_HPP
#define MAHJONG_WAITS_PREDICTOR_HPP

#include <array>
#include <bitset>
#include <mahjong/waits_predictor/states.hpp>
#include <mahjong/waits_predictor/value.hpp>

namespace mahjong::waits_predictor {
  using states::initialize;

  std::array<Value, 4u> predict_waits(const std::array<int, 34u>& wall,
                                      const std::bitset<34u>& river,
                                      int m,
                                      bool check = false);
  std::array<Value, 4u> predict_waits(const std::array<int, 34u>& wall,
                                      const std::array<int, 34u>& river,
                                      int m,
                                      bool check = false);
}

#endif
