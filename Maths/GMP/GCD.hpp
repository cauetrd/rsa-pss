#ifndef GCD_HPP
#define GCD_HPP

#include <gmpxx.h>
#include <tuple>

// Returns a tuple (g, x, y) such that a*x + b*y = g = gcd(a, b)
std::tuple<mpz_class, mpz_class, mpz_class> euclideanExtended(mpz_class a, mpz_class b);

mpz_class gcd(mpz_class a, mpz_class b);
#endif 