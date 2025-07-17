#include "salt_gen.hpp"

std::vector<unsigned char> generate_salt(size_t salt_length) {
    std::ifstream urandom("/dev/urandom", std::ios::in | std::ios::binary);
    if (!urandom) {
        std::cerr << "Failed to open /dev/urandom\n";
        return;
    }
    std::vector<unsigned char> salt(salt_length);
    urandom.read(reinterpret_cast<char*>(salt.data()), salt_length);

    return salt;
}