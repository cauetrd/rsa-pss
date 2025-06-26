#ifndef MOD_ARITHMETIC_HPP
#define MOD_ARITHMETIC_HPP

#include <gmpxx.h>
#include "EuclideanExtended.hpp"  // for modular inverse (needs gcd)

namespace mod{

    mpz_class add(const mpz_class& a, const mpz_class& b, const mpz_class& mod);

    mpz_class mul(const mpz_class& a, const mpz_class& b, const mpz_class& mod);

    mpz_class exp(mpz_class base, mpz_class exponent, const mpz_class& mod); // copies on purpose

    mpz_class inv(const mpz_class& a, const mpz_class& mod);

} 

#endif // MOD_ARITHMETIC_HPP
