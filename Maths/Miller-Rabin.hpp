#ifndef GMP_MILLER_RABIN_HPP
#define GMP_MILLER_RABIN_HPP

#include <gmpxx.h>
#include "modularArithmetics.hpp"

// Returns true if 'a' passes the Miller-Rabin primality test (probably prime), false otherwise
bool MillerRabin(mpz_class a);

#endif // GMP_MILLER_RABIN_HPP