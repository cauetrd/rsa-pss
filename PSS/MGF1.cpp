#include "MGF1.hpp"

std::vector<unsigned char> mgf1(const std::vector<unsigned char>& seed, size_t maskLen) {
    std::vector<unsigned char> mask(maskLen);
    size_t hLen = 32; // SHA3-256 output length in bytes

    size_t counter = 0;
    size_t generated = 0;
    while (generated < maskLen) {
        std::vector<unsigned char> C(4);
        C[0] = (counter >> 24) & 0xFF;
        C[1] = (counter >> 16) & 0xFF;
        C[2] = (counter >> 8) & 0xFF;
        C[3] = counter & 0xFF;

        std::vector<unsigned char> data(seed);
        data.insert(data.end(), C.begin(), C.end());

        std::vector<unsigned char> digest = hash_bytes_sha3_256(data);

        size_t toCopy = std::min(hLen, maskLen - generated);
        std::copy(digest.begin(), digest.begin() + toCopy, mask.begin() + generated);

        generated += toCopy;
        counter++;
    }
    return mask;
}