﻿# Implementation of RSA - PSS signature

# outer code used as of so far:

## bigInt Project on GitHub: https://github.com/faheel/BigInt (modified)

## GMP: https://gmplib.org/

(25/06/25) - switching BigInt for GMP; way better performance; check benchmark for exponentiation a^b mod m;

a = 12937129319831923138917129371293198319231389171293712931983192313891712937129319831923138917;

b = 1231874281471823714914248337123187428147182371491424833712318742814718237149142483371231874281471823714914248337;

m = 3123817318481783172834312381731848178317283431238173184817831728343123817318481783172834
![Benchmark Results](./images/benchmark.png)

## https://github.com/XKCP/XKCP SHA3-256

Using only Keccak-readable-and-compact.c modified.
