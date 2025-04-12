#include "states.hpp"
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <stdexcept>
using std::filesystem::path;

namespace waits_predictor::states {
  template <std::size_t N>
  void read_file(const std::filesystem::path& file,
                 const std::array<unsigned int, N>& num_hands,
                 const unsigned int length,
                 std::vector<Hands>& hands)
  {
    std::ifstream fin(file, std::ios_base::in | std::ios_base::binary);

    if (!fin) {
      throw std::runtime_error(std::format("Reading file does not exist: {}", file.string()));
    }

    for (std::size_t i = 0u; i < N; ++i) {
      Hands tmp(num_hands[i], Hand(length));

      for (std::size_t j = 0u; j < num_hands[i]; ++j) {
        fin.read(reinterpret_cast<char*>(tmp[j].data()), length * sizeof(Hand::value_type));
      }

      hands[i] = std::move(tmp);
    }
  }

  template <std::size_t N>
  void read_file(const std::filesystem::path& file,
                 const std::array<unsigned int, N>& num_hands,
                 const unsigned int length,
                 std::vector<HandWaits>& hand_waits)
  {
    std::ifstream fin(file, std::ios_base::in | std::ios_base::binary);

    if (!fin) {
      throw std::runtime_error(std::format("Reading file does not exist: {}", file.string()));
    }

    for (std::size_t i = 0u; i < N; ++i) {
      HandWaits tmp(num_hands[i], HandWait{Hand(length), 0u});

      for (std::size_t j = 0u; j < num_hands[i]; ++j) {
        fin.read(reinterpret_cast<char*>(tmp[j].hand.data()), length * sizeof(decltype(HandWait::hand)::value_type));
        fin.read(reinterpret_cast<char*>(&tmp[j].wait), sizeof(HandWait::wait));
      }

      hand_waits[i] = std::move(tmp);
    }
  }

  namespace lh {
    std::vector<std::vector<Hands>> hands_a(2u, std::vector<Hands>(5u));
    std::vector<std::vector<HandWaits>> hands_b(2u, std::vector<HandWaits>(5u));
    std::vector<std::vector<HandWaits>> hands_c(2u, std::vector<HandWaits>(5u));
    std::vector<std::vector<HandWaits>> hands_d(2u, std::vector<HandWaits>(5u));

    void initialize(const std::string& dir)
    {
      read_file(path(dir) / "lh_s_a.bin", num_hands_a[IND_SUITS], LEN_SUITS, hands_a[IND_SUITS]);
      read_file(path(dir) / "lh_h_a.bin", num_hands_a[IND_HONOR], LEN_HONOR, hands_a[IND_HONOR]);
      read_file(path(dir) / "lh_s_b.bin", num_hands_b[IND_SUITS], LEN_SUITS, hands_b[IND_SUITS]);
      read_file(path(dir) / "lh_h_b.bin", num_hands_b[IND_HONOR], LEN_HONOR, hands_b[IND_HONOR]);
      read_file(path(dir) / "lh_s_c.bin", num_hands_c[IND_SUITS], LEN_SUITS, hands_c[IND_SUITS]);
      read_file(path(dir) / "lh_s_d.bin", num_hands_d[IND_SUITS], LEN_SUITS, hands_d[IND_SUITS]);
      read_file(path(dir) / "lh_h_d.bin", num_hands_d[IND_HONOR], LEN_HONOR, hands_d[IND_HONOR]);
    }
  }

  namespace sp {
    std::vector<std::vector<Hands>> hands_a(2u, std::vector<Hands>(7u));
    std::vector<std::vector<HandWaits>> hands_b(2u, std::vector<HandWaits>(7u));

    void initialize(const std::string& dir)
    {
      read_file(path(dir) / "sp_s_a.bin", num_hands_a[IND_SUITS], LEN_SUITS, hands_a[IND_SUITS]);
      read_file(path(dir) / "sp_h_a.bin", num_hands_a[IND_HONOR], LEN_HONOR, hands_a[IND_HONOR]);
      read_file(path(dir) / "sp_s_b.bin", num_hands_b[IND_SUITS], LEN_SUITS, hands_b[IND_SUITS]);
      read_file(path(dir) / "sp_h_b.bin", num_hands_b[IND_HONOR], LEN_HONOR, hands_b[IND_HONOR]);
    }
  }

  namespace lh_sp {
    std::vector<std::vector<Hands>> hands_a(2u, std::vector<Hands>(5u));
    std::vector<std::vector<Hands>> hands_b(2u, std::vector<Hands>(5u));
    std::vector<std::vector<HandWaits>> hands_c(2u, std::vector<HandWaits>(5u));
    std::vector<std::vector<HandWaits>> hands_d(2u, std::vector<HandWaits>(5u));

    void initialize(const std::string& dir)
    {
      read_file(path(dir) / "lh_sp_s_a.bin", num_hands_a[IND_SUITS], LEN_SUITS, hands_a[IND_SUITS]);
      read_file(path(dir) / "lh_sp_h_a.bin", num_hands_a[IND_HONOR], LEN_HONOR, hands_a[IND_HONOR]);
      read_file(path(dir) / "lh_sp_s_b.bin", num_hands_b[IND_SUITS], LEN_SUITS, hands_b[IND_SUITS]);
      read_file(path(dir) / "lh_sp_h_b.bin", num_hands_b[IND_HONOR], LEN_HONOR, hands_b[IND_HONOR]);
      read_file(path(dir) / "lh_sp_s_c.bin", num_hands_c[IND_SUITS], LEN_SUITS, hands_c[IND_SUITS]);
      read_file(path(dir) / "lh_sp_s_d.bin", num_hands_d[IND_SUITS], LEN_SUITS, hands_d[IND_SUITS]);
      read_file(path(dir) / "lh_sp_h_d.bin", num_hands_d[IND_HONOR], LEN_HONOR, hands_d[IND_HONOR]);
    }
  }

  void initialize(const std::string& dir)
  {
    lh::initialize(dir);
    sp::initialize(dir);
    lh_sp::initialize(dir);
  }
}
