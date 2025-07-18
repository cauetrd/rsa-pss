#include "key_gen.hpp"

mpz_class randPrimeGen(std::ifstream& urandom){
    while (true){
    const size_t num_bytes = 128; // 1024 bits
    unsigned char buffer[num_bytes];
    
    urandom.read(reinterpret_cast<char*>(buffer), num_bytes);
    if (!urandom) {
        std::cerr << "Failed to read enough bytes\n";
        return 0;
    }

    buffer[num_bytes - 1] |= 1; // Ensure the last byte is odd to avoid even numbers

    // Initialize mpz_class from binary buffer
    mpz_class rand_num;
    mpz_import(rand_num.get_mpz_t(), num_bytes, 1, 1, 0, 0, buffer);

    if(MillerRabin(rand_num)){
        return rand_num;
        }
    }
}


void key_gen() {
    // Open /dev/urandom once for the entire session
    std::ifstream urandom("/dev/urandom", std::ios::in | std::ios::binary);
    if (!urandom) {
        std::cerr << "Failed to open /dev/urandom\n";
        return;
    }

    auto start = std::chrono::high_resolution_clock::now();

    mpz_class p = randPrimeGen(urandom);
    mpz_class q = randPrimeGen(urandom);

    mpz_class n = p * q;
    mpz_class phi = (p - 1) * (q - 1);
    mpz_class e = 65537; // Common prime public exponent
    mpz_class d = mod::inv(e, phi);

    // Write the keys to PEM files
    write_pem_file(n, e, "public_key.pem", "PUBLIC");
    write_pem_file(n, d, "private_key.pem", "PRIVATE");

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Key generation took " << elapsed.count() << " seconds.\n";

    return;
}