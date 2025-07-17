#include "hashFile.hpp"


std::vector<unsigned char> read_file_bytes(const std::string &filename) {
    std::ifstream file(filename, std::ios::binary);
    return std::vector<unsigned char>((std::istreambuf_iterator<char>(file)),
                                       std::istreambuf_iterator<char>());
}

std::vector<unsigned char> hash_file_sha3_256(const std::string &filename) {
    unsigned char output[32]; //32 bytes = 256 bits, adequate for sha_256;
    std::vector<unsigned char> file_data = read_file_bytes(filename);
    FIPS202_SHA3_256(file_data.data(), file_data.size(), output);

    return std::vector<unsigned char>(output, output + sizeof(output));
}

std::vector<unsigned char> hash_bytes_sha3_256(const std::vector<unsigned char> &data) {
    unsigned char output[32]; //32 bytes = 256 bits, adequate for sha_256;
    FIPS202_SHA3_256(data.data(), data.size(), output);

    return std::vector<unsigned char>(output, output + sizeof(output));
}

    