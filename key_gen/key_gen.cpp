#include <gmpxx.h>
#include <fstream>
#include <iostream>
#include "../Maths/GMP/Miller-Rabin.hpp"
#include <chrono>
#include "../Maths/GMP/modularArithmetics.hpp"
#include "../Maths/GMP/GCD.hpp" // for gcd function

mpz_class randPrimeGen(std::ifstream& urandom){
    while (true){
    const size_t num_bytes = 256; // 2048 bits
    unsigned char buffer[num_bytes];

    urandom.read(reinterpret_cast<char*>(buffer), num_bytes);
    if (!urandom) {
        std::cerr << "Failed to read enough bytes\n";
        return 0;
    }

    // Initialize mpz_class from binary buffer
    mpz_class rand_num;
    mpz_import(rand_num.get_mpz_t(), num_bytes, 1, 1, 0, 0, buffer);
    
    if(MillerRabin(rand_num)){
        return rand_num;
        }
    }
}

int main() {
    // Open /dev/urandom once for the entire session
    std::ifstream urandom("/dev/urandom", std::ios::in | std::ios::binary);
    if (!urandom) {
        std::cerr << "Failed to open /dev/urandom\n";
        return 1;
    }

    auto start = std::chrono::high_resolution_clock::now();

    mpz_class p, q;
    p = randPrimeGen(urandom);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Time taken to generate first prime: " << elapsed.count() << " seconds." << std::endl;
    start = std::chrono::high_resolution_clock::now();
    q = randPrimeGen(urandom);
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    std::cout << "Time taken to generate second prime: " << elapsed.count() << " seconds." << std::endl;
    start = std::chrono::high_resolution_clock::now();
    mpz_class n;

    n = p * q;
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    std::cout << "Time taken to calculate n: " << elapsed.count() << " seconds." << std::endl;
    start = std::chrono::high_resolution_clock::now();

    mpz_class phi;
    phi = (p - 1) * (q - 1);
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    std::cout << "Time taken to calculate phi: " << elapsed.count() << " seconds." << std::endl;

    std::cout << "n: " << n << std::endl;
    std::cout << "phi: " << phi << std::endl;

    mpz_class e(65537); // Common choice for e
    std::cout << "e: " << e << std::endl;
    
    // Ensure e is coprime with phi (obviously is, e is a prime number)
    // Adding this step for completeness
    if(gcd(e,phi) != 1) {
        std::cerr << "e is not coprime with phi, please choose a different e." << std::endl;
        return 1;
    }
    else{
        std::cout << "e is coprime with phi." << std::endl;
    }

    // Calculate d, the modular inverse of e mod phi
    mpz_class d = mod::inv(e, phi);    

    std::cout << "d: " << d << std::endl;


    //quick test:
    mpz_class m("12345678901234567890");
    std::cout << "Message: " << m << std::endl;

    mpz_class c = mod::exp(m, e, n);
    std::cout << "Ciphertext: " << c << std::endl;

    mpz_class decrypted_m = mod::exp(c, d, n);

    std::cout << "Decrypted message: " << decrypted_m << std::endl;

    if (m == decrypted_m) {
        std::cout << "Decryption successful!" << std::endl;
    } else {
        std::cout << "Decryption failed!" << std::endl;
    }
    return 0;
}