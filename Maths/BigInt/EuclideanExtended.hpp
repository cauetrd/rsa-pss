#ifndef EUCLIDEAN_EXTENDED_HPP
#define EUCLIDEAN_EXTENDED_HPP

#include "BigInt.hpp"
#include <tuple>

// Returns a tuple (g, x, y) such that a*x + b*y = g = gcd(a, b)
std::tuple<BigInt, BigInt, BigInt> euclideanExtended(BigInt a, BigInt b);

#endif // EUCLIDEAN_EXTENDED_HPP