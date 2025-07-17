#include "RSA-decrypt.hpp"

mpz_class rsa_decrypt(mpz_class c){
    std::pair<mpz_class, mpz_class> keys = read_pem_file("private_key.pem");

    mpz_class n = keys.first;
    mpz_class d = keys.second;

    mpz_class message = mod::exp(c, d, n);

    return message;
}