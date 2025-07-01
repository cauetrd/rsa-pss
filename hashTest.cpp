extern "C" {
    void FIPS202_SHA3_256(const unsigned char *input, unsigned int inputByteLen, unsigned char *output);
}

#include <iostream>
#include <iomanip>
#include <cstring>

int main() {
    const unsigned char message[] = "hello world";
    unsigned char hash[32];  // SHA3-256 → 32 bytes

    FIPS202_SHA3_256(message, sizeof(message) -1, hash);

    std::cout << "SHA3-256 hash: ";
    for (int i = 0; i < 32; ++i)
        std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    std::cout << std::endl;

    return 0;
}
    