#ifndef VALUE_HPP
#define VALUE_HPP

#include <cstdint>
#include <valarray>

namespace waits_predictor {
  constexpr unsigned int NUM_TIDS = 34u;

  struct Value {
    uint64_t all;
    std::valarray<uint64_t> each;

    Value() : all(0ull), each(UINT64_C(0), NUM_TIDS) {}
    Value& operator+=(const Value& rhs);
    Value& operator-=(const Value& rhs);
    std::valarray<double> props() const;
  };

  Value operator+(const Value& lhs, const Value& rhs);
  Value operator-(const Value& lhs, const Value& rhs);
}

#endif
