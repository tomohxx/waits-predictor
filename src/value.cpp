#include "value.hpp"

namespace waits_predictor {
  Value& Value::operator+=(const Value& rhs)
  {
    this->all += rhs.all;
    this->each += rhs.each;

    return *this;
  }

  Value& Value::operator-=(const Value& rhs)
  {
    this->all -= rhs.all;
    this->each -= rhs.each;

    return *this;
  }

  Value operator+(const Value& lhs, const Value& rhs)
  {
    return Value(lhs) += rhs;
  }

  Value operator-(const Value& lhs, const Value& rhs)
  {
    return Value(lhs) -= rhs;
  }

  std::valarray<double> Value::props() const
  {
    decltype(props()) ret(0., MAX_TIDS);

    if (this->all > 0u) {
      for (std::size_t i = 0u; i < MAX_TIDS; ++i) {
        ret[i] = static_cast<double>(each[i]) / all;
      }
    }

    return ret;
  }
}
