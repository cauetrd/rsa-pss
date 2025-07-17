#include "MGF1.hpp"
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

void print_hex(const std::vector<uint8_t>& data, const std::string& label) {
    std::cout << label << ": ";
    for (uint8_t byte : data) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
    }
    std::cout << std::dec << std::endl;
}

int main() {
    std::cout << "MGF1 Test Program" << std::endl;
    std::cout << "=================" << std::endl;
    
    // Test 1: Simple test with known seed
    std::vector<uint8_t> seed1 = {0x01, 0x02, 0x03, 0x04};
    std::vector<uint8_t> mask1 = mgf1(seed1, 20);
    
    print_hex(seed1, "Seed 1");
    print_hex(mask1, "Mask 1 (20 bytes)");
    std::cout << std::endl;
    
    // Test 2: Longer mask
    std::vector<uint8_t> seed2 = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
    std::vector<uint8_t> mask2 = mgf1(seed2, 50);
    
    print_hex(seed2, "Seed 2");
    print_hex(mask2, "Mask 2 (50 bytes)");
    std::cout << std::endl;
    
    // Test 3: Test with string data
    std::string test_string = "Hello, MGF1!";
    std::vector<uint8_t> seed3(test_string.begin(), test_string.end());
    std::vector<uint8_t> mask3 = mgf1(seed3, 32);
    
    std::cout << "Seed 3: \"" << test_string << "\"" << std::endl;
    print_hex(mask3, "Mask 3 (32 bytes)");
    std::cout << std::endl;
    
    // Test 4: Verify deterministic behavior
    std::vector<uint8_t> mask3_repeat = mgf1(seed3, 32);
    bool identical = (mask3 == mask3_repeat);
    std::cout << "Deterministic test: " << (identical ? "PASS" : "FAIL") << std::endl;
    
    return 0;
}
