#ifndef RSA_ENCRYPT_HPP
#define RSA_ENCRYPT_HPP

#include "PEM-read.hpp"
#include <iostream>
#include <gmpxx.h>
#include "../Maths/modularArithmetics.hpp"

mpz_class rsa_encrypt(mpz_class message, mpz_class n, mpz_class d);

#endif