#ifndef MGF1_HPP
#define MGF1_HPP

#include <vector>
#include <cstdint>

std::vector<uint8_t> mgf1(const std::vector<uint8_t>& seed, size_t maskLen, size_t hashLen = 32);

void mgf1_raw(const uint8_t* seed, size_t seedLen, uint8_t* mask, size_t maskLen, size_t hashLen = 32);

#endif // MGF1_HPP
