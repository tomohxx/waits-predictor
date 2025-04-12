#ifndef UTILS_HPP
#define UTILS_HPP

#include <cstdint>
#include <functional>
#include <vector>

bool iswh0(const std::vector<uint8_t>& hand);
bool iswh2(std::vector<uint8_t>& hand);
bool iswhh(const std::vector<uint8_t>& hand);
bool issp(const std::vector<uint8_t>& hand);
uint16_t isrh(std::vector<uint8_t>& hand, std::function<bool(std::vector<uint8_t>&)> func);

#endif
