#include "MGF1.hpp"
#include "../hash/Keccak.h"
#include <cstring>
#include <algorithm>

/**
 * Convert a 32-bit integer to a 4-byte big-endian representation
 */
static void i2osp(uint32_t x, uint8_t* output) {
    output[0] = (x >> 24) & 0xFF;
    output[1] = (x >> 16) & 0xFF;
    output[2] = (x >> 8) & 0xFF;
    output[3] = x & 0xFF;
}

/**
 * Hash function wrapper for SHA3-256 using Keccak
 */
static void hash_sha3_256(const uint8_t* input, size_t inputLen, uint8_t* output) {
    // SHA3-256: rate=1088, capacity=512, delimited suffix=0x06 for SHA3
    Keccak(1088, 512, input, inputLen, 0x06, output, 32);
}

std::vector<uint8_t> mgf1(const std::vector<uint8_t>& seed, size_t maskLen, size_t hashLen) {
    std::vector<uint8_t> mask(maskLen);
    mgf1_raw(seed.data(), seed.size(), mask.data(), maskLen, hashLen);
    return mask;
}

void mgf1_raw(const uint8_t* seed, size_t seedLen, uint8_t* mask, size_t maskLen, size_t hashLen) {
    // Step 1: Check if maskLen >= 2^32 * hashLen (in practice, this won't happen)
    if (maskLen >= (1ULL << 32) * hashLen) {
        // mask too long error - but we'll just proceed for practical purposes
        return;
    }
    
    // Step 2: Let T be the empty octet string
    size_t tLen = 0;
    
    // Step 3: For counter from 0 to ceil(maskLen / hashLen) - 1
    size_t iterations = (maskLen + hashLen - 1) / hashLen;
    
    std::vector<uint8_t> temp(seedLen + 4); // seed || C
    std::memcpy(temp.data(), seed, seedLen);
    
    for (uint32_t counter = 0; counter < iterations; counter++) {
        // Step 3.a: Convert counter to an octet string C of length 4 octets
        i2osp(counter, temp.data() + seedLen);
        
        // Step 3.b: T = T || Hash(seed || C)
        uint8_t hash_output[32]; // Assuming SHA3-256 (32 bytes)
        hash_sha3_256(temp.data(), temp.size(), hash_output);
        
        // Copy the appropriate number of bytes to the mask
        size_t bytes_to_copy = std::min(hashLen, maskLen - tLen);
        std::memcpy(mask + tLen, hash_output, bytes_to_copy);
        tLen += bytes_to_copy;
        
        if (tLen >= maskLen) {
            break;
        }
    }
    
    // Step 4: Return the leading maskLen octets of T as the octet string mask
    // (This is already done in the loop above)
}
