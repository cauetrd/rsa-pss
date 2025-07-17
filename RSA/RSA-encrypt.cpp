#include "RSA-encrypt.hpp"

mpz_class rsa_encrypt(mpz_class message, mpz_class n, mpz_class d) {
    
    mpz_class c = mod::exp(message, d, n); // c = m^d (mod n);

    return c;
}


