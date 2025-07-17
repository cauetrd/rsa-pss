#include "PSS_verification.hpp"

//returns true if the message is consistent, false otherwise
bool PSS_verify(std::vector<unsigned char> M, std::vector<unsigned char> EM, int emBits){

    std::vector<unsigned char> mHash;
    mHash = hash_bytes_sha3_256(M);
    int hLen = mHash.size(); // hLen = 32, length of the hash
    
    int sLen = hLen; // Assuming sLen is the same as hLen as used in PSS_encode

    int emLen = (emBits + 7) / 8; // = ceiling (emBits)/8)
    
    if(EM.size() != emLen) {
        std::cerr << "Inconsistent" << std::endl;
        return false;
    }

    if(emLen < hLen + sLen + 2) {
        std::cerr << "Inconsistent" << std::endl;
        return false;
    }  

    // Rightmost octet of EM does not have hexadecimal value 0xbc
    
    if(EM[emLen-1] != 0xBC) {
        std::cerr << "Inconsistent" << std::endl;
        return false;
    }

    int DBlen = emLen - hLen - 1; // Length of the data block

    //maskedDB = EM[0:DBlen]
    std::vector<unsigned char> maskedDB(EM.begin(), EM.begin() + DBlen);

    //H = EM[DBlen:DBlen + hLen]
    std::vector<unsigned char> H(EM.begin() + DBlen, EM.begin() + DBlen + hLen);

    // 8*emLen - emBits bits of the leftmost octet in maskedDB must be 0;
    for(size_t i = 0; i<(8 * emLen - emBits); i++) {
        unsigned char byte = maskedDB[i / 8];
        bool bit = byte & (1 << (7 - (i % 8)));
        if(bit != 0) {
            std::cerr << "Inconsistent" << std::endl;
            return false;
        }
    }

    std::vector<unsigned char> dbMask = mgf1(H, DBlen);

    std::vector<unsigned char> DB = xorVectors(maskedDB, dbMask);

    for(size_t i = 0; i < (8*emLen - emBits); i++) {
        DB[i/8] &= ~(1 << (7 - (i % 8))); // Sets bit to 0;
    }

    // Check emLen - hLen - sLen - 2 leftmost bytes
    int zeroLen = emLen - hLen - sLen - 2;
    for (int i = 0; i < zeroLen; ++i) {
        if (DB[i] != 0x00) {
            std::cerr << "Inconsistent" << std::endl;
            return false;
        }
    }

    // Check the first byte after the zero bytes
    if (DB[zeroLen] != 0x01) {
        std::cerr << "Inconsistent" << std::endl;
        return false;
    }

    // Extract salt
    std::vector<unsigned char> salt(DB.end() - sLen, DB.end());

    // Construct M'
    std::vector<unsigned char> M_prime(8 + hLen + sLen, 0x00);
    std::copy(mHash.begin(), mHash.end(), M_prime.begin() + 8);
    std::copy(salt.begin(), salt.end(), M_prime.begin() + 8 + hLen);

    // Step 13: Hash M'
    std::vector<unsigned char> H_prime = hash_bytes_sha3_256(M_prime);

    // Step 14: Compare H and H'
    if (H == H_prime) {
        std::cout << "Consistent" << std::endl;
        return true;
    } else {
        std::cerr << "Inconsistent" << std::endl;
        return false;
    }
}