#include <gmpxx.h>
#include <fstream>
#include <vector>
#include <string>
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

std::string base64_encode(const std::vector<unsigned char>& in) {
    static const char *base64_chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";

    std::string out;
    int val = 0, valb = -6;
    for (unsigned char c : in) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            out.push_back(base64_chars[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    if (valb > -6) out.push_back(base64_chars[((val << 8) >> valb) & 0x3F]);
    while (out.size() % 4) out.push_back('=');
    return out;
}

std::string to_pem(const std::string& base64_data, const std::string& label) {
    std::string pem = "-----BEGIN " + label + "-----\n";
    for(size_t pos = 0; pos < base64_data.size(); pos += 64) {
        pem += base64_data.substr(pos, 64) + "\n"; // 64 characters per line
    }
    pem += "-----END " + label + "-----\n";
    return pem;
}

void write_pem_file(const mpz_class& n, const mpz_class& key, const std::string& filename, const std::string& label) {
    // Calculate required buffer sizes
    size_t n_count = (mpz_sizeinbase(n.get_mpz_t(), 2) + 7) / 8; // Convert bits to bytes
    size_t key_count = (mpz_sizeinbase(key.get_mpz_t(), 2) + 7) / 8;
    
    // Allocate buffers
    std::vector<unsigned char> n_bytes(n_count);
    std::vector<unsigned char> key_bytes(key_count);

    // Export the numbers to byte arrays
    size_t actual_n_count = 0, actual_key_count = 0;
    mpz_export(n_bytes.data(), &actual_n_count, 1, 1, 0, 0, n.get_mpz_t());
    mpz_export(key_bytes.data(), &actual_key_count, 1, 1, 0, 0, key.get_mpz_t());
    
    // Resize vectors to actual sizes
    n_bytes.resize(actual_n_count);
    key_bytes.resize(actual_key_count);

    std::vector<unsigned char> combined_bytes;

    uint32_t n_size = n_bytes.size();
    uint32_t key_size = key_bytes.size();

    // Combine n and key into a single byte vector
    // structure: [n_size (4 bytes)][n_bytes][key_size (4 bytes)][key_bytes]
    combined_bytes.insert(combined_bytes.end(), reinterpret_cast<unsigned char*>(&n_size), reinterpret_cast<unsigned char*>(&n_size) + sizeof(n_size));
    combined_bytes.insert(combined_bytes.end(), n_bytes.begin(), n_bytes.end());
    combined_bytes.insert(combined_bytes.end(), reinterpret_cast<unsigned char*>(&key_size), reinterpret_cast<unsigned char*>(&key_size) + sizeof(key_size));
    combined_bytes.insert(combined_bytes.end(), key_bytes.begin(), key_bytes.end());

    std::string base64_data = base64_encode(combined_bytes);
    std::string pem_data = to_pem(base64_data, "RSA " + label + " KEY");

    std::ofstream out(filename);
    if (!out) {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
        return;
    }
    out << pem_data;
    out.close();

    std::cout << "PEM file written: " << filename << std::endl;
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

    write_pem_file(e, n, "public_key.pem", "PUBLIC");
    write_pem_file(n, d, "private_key.pem", "PRIVATE");

    return 0;
}