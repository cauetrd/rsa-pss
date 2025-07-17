#include "RSAPSS_verify.hpp"


bool RSAPSS_verify(const std::vector<unsigned char>& M, const std::vector<unsigned char>& S){
    std::string public_key_file = "public_key.pem";
    std::pair<mpz_class, mpz_class> public_key = read_pem_file(public_key_file);

    mpz_class n = public_key.first;
    mpz_class e = public_key.second;

    int modulus_bits = mpz_sizeinbase(n.get_mpz_t(), 2); // Get the number of bits in key

    if(S.size() != (modulus_bits + 7) / 8) {
        std::cerr << "Signature size does not match expected size." << std::endl;
        return false;
    }

    mpz_class s;
    mpz_import(s.get_mpz_t(), S.size(), 1, 1, 0, 0, S.data()); // Convert signature from byte vector to mpz_class

    mpz_class m = rsa_decrypt(s, n, e); // Decrypt the signature to get the message

    std::vector<unsigned char> EM;
    int emBits = modulus_bits-1;
    int emLen = (emBits + 7) / 8; // = ceiling (emBits)/8

    EM.resize(emLen);
    
    std::fill(EM.begin(), EM.end(), 0); // Zero-fill
    size_t mLen = (mpz_sizeinbase(m.get_mpz_t(), 2) + 7) / 8;
    mpz_export(EM.data() + (emLen - mLen), nullptr, 1, 1, 0, 0, m.get_mpz_t());

    bool isValid = PSS_verify(M, EM, emBits); // Verify the message against the encoded message

    if(isValid) {
        std::cout << "Signature is valid." << std::endl;
        return true;
    } else {
        std::cout << "Signature is invalid." << std::endl;
        return false;
    }

}