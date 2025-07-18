#include "RSAPSS_sign.hpp"

std::string RSAPSS_sign(std::vector<unsigned char>& M){
    std::string private_key_file = "private_key.pem";

    std::pair<mpz_class, mpz_class> private_key = read_pem_file(private_key_file);
    mpz_class n = private_key.first;
    mpz_class d = private_key.second;

    int modulus_bits =  mpz_sizeinbase(n.get_mpz_t(),2); // Get the number of bits in key
    int emBits = modulus_bits - 1; // For RSA-PSS, emBits = modulus_bits - 1

    std::vector<unsigned char> EM = PSS_encode(M, emBits); // Encode the message M into EM

    mpz_class m;
    mpz_import(m.get_mpz_t(), EM.size(), 1, 1, 0, 0, EM.data()); // Convert EM from byte vector to mpz_class

    if(m < 0 or m >=n){
        std::cerr << "Message is out of range for RSA encryption." << std::endl;
        return {}; // Return empty vector if message is invalid
    }

    mpz_class s = rsa_encrypt(m, n, d);


    int signatureLen = (modulus_bits + 7) / 8; 
    std::vector<unsigned char> signature(signatureLen, 0);

    size_t s_size = (mpz_sizeinbase(s.get_mpz_t(), 2) + 7) / 8;
    mpz_export(signature.data() + (signatureLen - s_size), nullptr, 1, 1, 0, 0, s.get_mpz_t());

    return base64_encode(signature.data(), signature.size()); // Return the signature
}   