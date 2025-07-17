#include "utils.hpp"


std::vector<unsigned char> xorVectors(const std::vector<unsigned char>& a, const std::vector<unsigned char>& b) {
    if (a.size() != b.size()) {
        throw std::invalid_argument("Vectors must be of the same size for XOR operation");
    }
    std::vector<unsigned char> result(a.size());
    for (size_t i = 0; i < a.size(); ++i) {
        result[i] = a[i] ^ b[i];
    }
    return result;
}