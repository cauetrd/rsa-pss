#ifndef MILLER_RABIN_HPP
#define MILLER_RABIN_HPP

#include <gmpxx.h>
#include "modularArithmetics.hpp"

// Returns true if 'a' passes the Miller-Rabin primality test (probably prime), false otherwise
bool MillerRabin(mpz_class a);

#endif // MILLER_RABIN_HPP