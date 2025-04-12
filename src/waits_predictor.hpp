#ifndef WAITS_PREDICTOR_HPP
#define WAITS_PREDICTOR_HPP

#include "states.hpp"
#include "value.hpp"
#include <array>

namespace waits_predictor {
  using states::initialize;

  std::array<Value, 4u> predict_waits(const std::vector<int>& wall,
                                      const std::vector<int>& river,
                                      int m,
                                      bool check = false);
}

#endif
