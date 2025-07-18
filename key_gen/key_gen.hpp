#ifndef KEY_GEN_HPP
#define KEY_GEN_HPP

#include <gmpxx.h>
#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <chrono>
#include "../Maths/modularArithmetics.hpp"
#include "../Maths/Miller-Rabin.hpp"
#include "../PEM/PEM-write.hpp"

    mpz_class randPrimeGen(std::ifstream& urandom);
    void key_gen();

#endif