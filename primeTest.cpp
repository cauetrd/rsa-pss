#include "./Maths/GMP/Miller-Rabin.hpp"

#include <iostream>
#include <gmpxx.h>
#include <cstring>
#include <cstdlib>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

// Base64 decoding function
vector<unsigned char> base64_decode(const string& input) {
    static const char* base64_chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";

    vector<unsigned char> result;
    int val = 0, valb = -8;
    
    for (char c : input) {
        if (c == '=' || c == '\n' || c == '\r' || c == ' ') continue;
        
        const char* pos = strchr(base64_chars, c);
        if (!pos) continue;
        
        val = (val << 6) + (pos - base64_chars);
        valb += 6;
        if (valb >= 0) {
            result.push_back((val >> valb) & 0xFF);
            valb -= 8;
        }
    }
    return result;
}

// Function to read and parse PEM file
pair<mpz_class, mpz_class> read_pem_file(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Failed to open file: " << filename << endl;
        return make_pair(mpz_class(0), mpz_class(0));
    }
    
    string line, base64_content;
    bool reading_key = false;
    
    // Extract base64 content from PEM
    while (getline(file, line)) {
        if (line.find("-----BEGIN") != string::npos) {
            reading_key = true;
            continue;
        }
        if (line.find("-----END") != string::npos) {
            break;
        }
        if (reading_key) {
            base64_content += line;
        }
    }
    
    // Decode base64 to binary
    vector<unsigned char> binary_data = base64_decode(base64_content);
    
    // Parse your custom format: [n_size][n_bytes][key_size][key_bytes]
    if (binary_data.size() < 8) {
        cerr << "Invalid key file format" << endl;
        return make_pair(mpz_class(0), mpz_class(0));
    }
    
    // Read n_size (first 4 bytes)
    uint32_t n_size = *reinterpret_cast<uint32_t*>(binary_data.data());
    
    // Read n (next n_size bytes)
    mpz_class n;
    mpz_import(n.get_mpz_t(), n_size, 1, 1, 0, 0, binary_data.data() + 4);
    
    // Read key_size (next 4 bytes)
    uint32_t key_size = *reinterpret_cast<uint32_t*>(binary_data.data() + 4 + n_size);
    
    // Read key (next key_size bytes)
    mpz_class key;
    mpz_import(key.get_mpz_t(), key_size, 1, 1, 0, 0, binary_data.data() + 8 + n_size);
        
    return make_pair(n, key);
}


int main(){
    // Read public key (n, e)
    auto [n_pub, e] = read_pem_file("public_key.pem");
    cout << "Public key loaded:" << endl;
    cout << "n: " << n_pub << endl;
    cout << "e: " << e << endl;
    
    // Read private key (n, d)
    auto [n_priv, d] = read_pem_file("private_key.pem");
    cout << "\nPrivate key loaded:" << endl;
    cout << "n: " << n_priv << endl;
    cout << "d: " << d << endl;
    
    // Verify n is the same in both keys
    if (n_pub == n_priv) {
        cout << "\nKeys are consistent!" << endl;
    } else {
        cout << "\nError: Keys have different modulus!" << endl;
    }

    // Test if encryption works

    cout << endl << "Testing encryption and decryption..." << endl;

    mpz_class message("1234567890123456789012345678901234567890");
    cout << "Message: " << message << endl;
    // Encrypt using public key: c = m^e mod n
    mpz_class c = mod::exp(message, e, n_pub);
    cout << "Ciphertext: " << c << endl;
    // Decrypt using private key: m = c^d mod n
    mpz_class decrypted_message = mod::exp(c, d, n_priv);
    cout << "Decrypted message: " << decrypted_message << endl;
    if (message == decrypted_message) {
        cout << "Decryption successful!" << endl;
    } else {
        cout << "Decryption failed!" << endl;
    }
    return 0;
}