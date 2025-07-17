#include "../hash/hashFile.hpp"
#include <cstring>
#include <iostream>
#include <vector>


//EMlen = 256;

int main(){
    int EMLEN = 256; // Length of the mask in bytes (na verdade é size de n);
    int SALT_LENGTH = 32; // Length of the salt in bytes
    int HASH_LENGTH = 32; // Length of the hash output in bytes
    std::string filename = "vascao.png"; // Example file to hash

    std::vector<unsigned char> mHash = hash_file_sha3_256(filename);
    std::vector<unsigned char> salt = generate_salt(SALT_LENGTH);
    std::vector<unsigned char> EM(EMLEN, 0); // Initialize the mask with zeros
    std::vector<unsigned char> mLInha(8, 0x00); // mLInha is initialized to 8 bytes of zeros
    mLInha.insert(mLInha.end(), mHash.begin(), mHash.end()); // Append the hash to mLInha
    mLInha.insert(mLInha.end(), salt.begin(), salt.end()); // Append the salt to mLInha
    std::vector<unsigned char> H = hash_bytes_sha3_256(std::string(mLInha.begin(), mLInha.end())); // Hash mLInha
    std::vector<unsigned char> paddingString(EMLEN - mHash.size() - salt.size() - 2, 0x00); // Padding with zeros
    std::vector<unsigned char> DB;
    DB.insert(DB.end(), paddingString.begin(), paddingString.end()); // Add padding
    DB.insert(DB.end(), 0x01); // Add 0x01 byte
    DB.insert(DB.end(), salt.begin(), salt.end()); // Add salt

    std::vector<unsigned char> mask = mgf1(H, EMLEN - 1 - HASH_LENGTH, HASH_LENGTH); // Generate mask using MGF1
    
}

