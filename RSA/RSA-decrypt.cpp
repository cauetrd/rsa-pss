#include "RSA-decrypt.hpp"

mpz_class rsa_decrypt(mpz_class c, mpz_class n, mpz_class e) {
    mpz_class message = mod::exp(c, e, n);
    return message;
}