#include "PEM-read.hpp"
#include <iostream>
#include <gmpxx.h>
#include "Maths/GMP/modularArithmetics.hpp"

mpz_class rsa_encrypt(mpz_class message){

    std::pair<mpz_class, mpz_class> keys = read_pem_file("public_key.pem");
    
    mpz_class n = keys.first;
    mpz_class e = keys.second;

    mpz_class c = mod::exp(message, e, n); // c = m^e (mod n);

    return c;
}


