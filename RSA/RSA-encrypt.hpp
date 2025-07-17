#ifndef RSA_ENCRYPT_HPP
#define RSA_ENCRYPT_HPP

#include "PEM-read.hpp"
#include <iostream>
#include <gmpxx.h>
#include "Maths/GMP/modularArithmetics.hpp"

mpz_class rsa_encrypt(mpz_class message);

#endif