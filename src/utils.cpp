// https://tomohxx.github.io/mahjong-algorithm-book/winning_hand/
#include "utils.hpp"
#include <algorithm>

bool iswh0(const std::vector<uint8_t>& hand)
{
  uint8_t a = hand[0], b = hand[1];

  for (std::size_t i = 0u; i < hand.size() - 2u; ++i) {
    if (const uint8_t r = a % 3; b >= r && hand[i + 2] >= r) {
      a = b - r;
      b = hand[i + 2] - r;
    }
    else return false;
  }
  return a % 3 == 0 && b % 3 == 0;
}

bool iswh2(std::vector<uint8_t>& hand)
{
  uint8_t s = 0u;

  for (std::size_t i = 0u; i < hand.size(); ++i) {
    s += i * hand[i];
  }

  for (std::size_t p = s * 2u % 3u; p < hand.size(); p += 3u) {
    if (hand[p] >= 2) {
      hand[p] -= 2;

      if (iswh0(hand)) {
        hand[p] += 2;
        return true;
      }
      else hand[p] += 2;
    }
  }
  return false;
}

bool iswhh(const std::vector<uint8_t>& hand)
{
  bool head = false;

  for (std::size_t i = 0u; i < hand.size(); ++i) {
    if (const uint8_t r = hand[i] % 3; r == 0) {
      continue;
    }
    else if (r == 1) {
      return false;
    }
    else if (r == 2 && !head) {
      head = true;
    }
    else return false;
  }

  return true;
}

bool issp(const std::vector<uint8_t>& hand)
{
  return std::all_of(hand.begin(), hand.end(), [](const uint8_t& x) { return x == 0u || x == 2u; });
}

uint16_t isrh(std::vector<uint8_t>& hand, std::function<bool(std::vector<uint8_t>&)> func)
{
  uint16_t wait = 0u;

  for (std::size_t i = 0u; i < hand.size(); ++i) {
    if (hand[i] < 4u) {
      ++hand[i];

      if (func(hand)) {
        wait ^= 1u << i;
      }
      --hand[i];
    }
  }

  return wait;
}
