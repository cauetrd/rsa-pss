#include "Miller-Rabin.hpp"
#include <iostream>
#include <chrono>


//returns true if a is probably prime and false if a is definetly composite
//for each iteration, chances of identifying a composite as prime is <= 1/4
//chances for misidentification of algorithm is (1/4)^k, k being the number of iterations
// ...existing code...
bool MillerRabin(BigInt a){
    int iteration = 40;

    if(a < 2 || (a != 2 && a % 2 == 0)) return false;
    if(a <= 3) return true;

    BigInt d = a - 1;
    int s = 0;
    while(d % 2 == 0){
        d /= 2;
        s++;
    }

    for(int i = 0; i < iteration; i++){
        auto start = std::chrono::high_resolution_clock::now();
        std::cout << "Iteration " << i + 1 << " at: " << start.time_since_epoch().count() << std::endl;

        BigInt rand = big_random(50);
        if(rand < 0) rand = -rand;
        BigInt test = rand % (a - 3) + 2;

        std::cout << "sending " << test << " " << d << " to mod::exp" << std::endl;
        BigInt x = mod::exp(test, d, a);
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
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        std::cout << "Miller-Rabin test took " << elapsed.count() << " seconds." << std::endl;
        
        if(continue_outer)
        continue;
        
        return false; // composite
    }

    return true; // probably prime
}

