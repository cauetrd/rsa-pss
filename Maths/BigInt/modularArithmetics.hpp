#ifndef MOD_ARITHMETIC_HPP
#define MOD_ARITHMETIC_HPP

#include "BigInt.hpp"          // your big-integer class
#include "GCD.hpp"  // for modular inverse (needs gcd)

namespace mod{

    BigInt add(const BigInt& a, const BigInt& b, const BigInt& mod);

    BigInt mul(const BigInt& a, const BigInt& b, const BigInt& mod);

    BigInt exp(BigInt base, BigInt exponent, const BigInt& mod); // copies on purpose

    BigInt inv(const BigInt& a, const BigInt& mod);

} 

#endif // MOD_ARITHMETIC_HPP
