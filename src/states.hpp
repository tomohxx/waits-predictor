#ifndef STATES_HPP
#define STATES_HPP

#include <array>
#include <cstdint>
#include <string>
#include <utility>
#include <vector>

namespace waits_predictor {
  constexpr unsigned int IND_SUITS = 0u;
  constexpr unsigned int IND_HONOR = 1u;
  constexpr unsigned int LEN_SUITS = 9u;
  constexpr unsigned int LEN_HONOR = 7u;
  using Hand = std::vector<uint8_t>;
  using Hands = std::vector<Hand>;

  struct HandWait {
    Hand hand;
    uint16_t wait;
  };

  using HandWaits = std::vector<HandWait>;

  namespace states {
    namespace lh {
      constexpr std::array<std::array<unsigned int, 5u>, 2u> num_hands_a = {{
          {{1u, 16u, 127u, 627u, 2098u}},
          {{1u, 7u, 21u, 35u, 35u}},
      }};

      constexpr std::array<std::array<unsigned int, 5u>, 2u> num_hands_b = {{
          {{9u, 135u, 996u, 4475u, 0u}},
          {{7u, 42u, 105u, 140u, 0u}},
      }};

      constexpr std::array<std::array<unsigned int, 5u>, 2u> num_hands_c = {{
          {{15u, 202u, 1261u, 4678u, 0u}},
          {{0u, 0u, 0u, 0u, 0u}},
      }};

      constexpr std::array<std::array<unsigned int, 5u>, 2u> num_hands_d = {{
          {{9u, 249u, 2406u, 12533u, 40070u}},
          {{7u, 63u, 210u, 350u, 315u}},
      }};

      extern std::vector<std::vector<Hands>> hands_a;
      extern std::vector<std::vector<HandWaits>> hands_b;
      extern std::vector<std::vector<HandWaits>> hands_c;
      extern std::vector<std::vector<HandWaits>> hands_d;

      void initialize(const std::string& dir);
    }

    namespace sp {
      constexpr std::array<std::array<unsigned int, 7u>, 2u> num_hands_a = {{
          {{1u, 9u, 36u, 84u, 126u, 126u, 84u}},
          {{1u, 7u, 21u, 35u, 35u, 21u, 7u}},
      }};

      constexpr std::array<std::array<unsigned int, 7u>, 2u> num_hands_b = {{
          {{9u, 72u, 252u, 504u, 630u, 504u, 252u}},
          {{7u, 42u, 105u, 140u, 105u, 42u, 7u}},
      }};

      extern std::vector<std::vector<Hands>> hands_a;
      extern std::vector<std::vector<HandWaits>> hands_b;

      void initialize(const std::string& dir);
    }

    namespace lh_sp {
      constexpr std::array<std::array<unsigned int, 5u>, 2u> num_hands_a = {{
          {{1u, 0u, 7u, 0u, 10u}},
          {{1u, 0u, 0u, 0u, 0u}},
      }};

      constexpr std::array<std::array<unsigned int, 5u>, 2u> num_hands_b = {{
          {{9u, 0u, 36u, 0u, 0u}},
          {{7u, 0u, 0u, 0u, 0u}},
      }};

      constexpr std::array<std::array<unsigned int, 5u>, 2u> num_hands_c = {{
          {{0u, 21u, 0u, 60u, 0u}},
          {{0u, 0u, 0u, 0u, 0u}},
      }};

      constexpr std::array<std::array<unsigned int, 5u>, 2u> num_hands_d = {{
          {{9u, 0u, 144u, 0u, 126u}},
          {{7u, 0u, 0u, 0u, 0u}},
      }};

      extern std::vector<std::vector<Hands>> hands_a;
      extern std::vector<std::vector<Hands>> hands_b;
      extern std::vector<std::vector<HandWaits>> hands_c;
      extern std::vector<std::vector<HandWaits>> hands_d;

      void initialize(const std::string& dir);
    }

    void initialize(const std::string& dir);
  }
}

#endif
