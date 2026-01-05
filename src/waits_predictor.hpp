#ifndef WAITS_PREDICTOR_HPP
#define WAITS_PREDICTOR_HPP

#include "states.hpp"
#include "value.hpp"
#include <array>
#include <bitset>

namespace waits_predictor {
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
