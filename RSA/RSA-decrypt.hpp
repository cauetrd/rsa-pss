#ifndef RSA_DECRYPT_HPP
#define RSA_DECRYPT_HPP

#include <iostream>
#include <gmpxx.h>
#include "../Maths/modularArithmetics.hpp"

    mpz_class rsa_decrypt(mpz_class c, mpz_class n, mpz_class e);

#endif