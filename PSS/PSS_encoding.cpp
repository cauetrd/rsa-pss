#include "PSS_encoding.hpp"

std::vector<unsigned char> PSS_encode(const std::vector<unsigned char>& M, int emBits) {

    std::vector<unsigned char> mHash = hash_bytes_sha3_256(M);
    int hLen = mHash.size(); // hLen = 32, length of the hash in bytes
    
    int sLen = hLen; // Assuming sLen is the same as hLen for simplicity

    int emLen = (emBits + 7) / 8; // = ceiling (emBits)/8)

    if(emLen < hLen + sLen + 2) {
        std::cerr << "Encoding error: emLen is too small for the given hash and salt lengths." << std::endl;
        return {};
    }

    std::ifstream urandom("/dev/urandom", std::ios::in | std::ios::binary);
    if (!urandom) {
        std::cerr << "Failed to open /dev/urandom\n";
        return {};
    }
    // Generate random salt
    std::vector<unsigned char> salt(sLen);
    urandom.read(reinterpret_cast<char*>(salt.data()), sLen);
    if (!urandom) {
        std::cerr << "Failed to read enough bytes for salt\n";
        return {};
    }

    //empty 8 bytes padding string
    std::vector<unsigned char> emptyString(8,(unsigned char)0x00);

    // Create the message M' = empty || Hash(M) || salt
    std::vector<unsigned char> M_prime;
    M_prime.reserve(8 + hLen + sLen);
    M_prime.insert(M_prime.end(), emptyString.begin(), emptyString.end());
    M_prime.insert(M_prime.end(), mHash.begin(), mHash.end());
    M_prime.insert(M_prime.end(), salt.begin(), salt.end());

    // H = Hash(M')
    std::vector<unsigned char> H = hash_bytes_sha3_256(M_prime);

    size_t PSLen = emLen - hLen - sLen - 2; // Length of the padding string
    std::vector<unsigned char> PS(PSLen, (unsigned char)0x00);

    size_t DBLen = PSLen + 1 + sLen; // Length of the data block

    // Create the data block DB = PS || 0x01 || salt
    std::vector<unsigned char> DB;
    DB.reserve(DBLen);
    DB.insert(DB.end(), PS.begin(), PS.end());
    DB.push_back(0x01);
    DB.insert(DB.end(), salt.begin(), salt.end());

    // Create and apply the mask to the data block
    std::vector<unsigned char> dbMask = mgf1(H, DBLen);
    std::vector<unsigned char> maskedDB = xorVectors(DB, dbMask);

    // Set the leftmost 8emLen - emBits bits of the leftmost octet in maskedDB to zero.
    for(size_t i = 0; i<(8 * emLen - emBits); i++) {
        maskedDB[i / 8] &= ~(1 << (7 - (i % 8))); // Clear the bit
    }

    std::vector<unsigned char> EM;
    EM.reserve(emLen);

    // EM = maskedDB || H || 0xBC
    EM.insert(EM.end(), maskedDB.begin(), maskedDB.end());
    EM.insert(EM.end(), H.begin(), H.end());
    EM.push_back(0xBC); // Add the 0xBC byte at the end
    
    return EM; // Return the encoded message
}   


