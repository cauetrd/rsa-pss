#include "Miller-Rabin.hpp"
#include <iostream>
#include <chrono>

//returns true if a is probably prime and false if a is definetly composite
//for each iteration, chances of identifying a composite as prime is <= 1/4
//chances for misidentification of algorithm is (1/4)^k, k being the number of iterations
// ...existing code...
bool MillerRabin(mpz_class a){
    int iteration = 40;

    if(a < 2 || (a != 2 && a % 2 == 0)) return false;
    if(a <= 3) return true;

    mpz_class d = a - 1;
    int s = 0;
    while(d % 2 == 0){
        d /= 2;
        s++;
    }

    for(int i = 0; i < iteration; i++){
        gmp_randclass rand_gen(gmp_randinit_default); //gmp function for random number generation
        rand_gen.seed(time(nullptr) + i); // seed with current time and iteration number

        mpz_class test = rand_gen.get_z_range(a - 3) + 2; // generate random number in range [2, a-1]

        mpz_class x = mod::exp(test, d, a);
        if(x == 1 || x == a - 1)
            continue;

        bool continue_outer = false;
        for(int r = 1; r < s; r++){
            x = mod::mul(x, x, a);
            if(x == a - 1){
                continue_outer = true;
                break;
            }
        }

        if(continue_outer)
        continue;
        
        return false; // composite
    }

    return true; // probably prime
}

