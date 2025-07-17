#ifndef RSA_DECRYPT_HPP
#define RSA_DECRYPT_HPP

#include "PEM-read.hpp"
#include <iostream>
#include <gmpxx.h>
#include "Maths/GMP/modularArithmetics.hpp"

    mpz_class rsa_decrypt(mpz_class c);

#endif