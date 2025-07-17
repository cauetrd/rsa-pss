#include "PEM-read.hpp"

// Base64 decoding function
std::vector<unsigned char> base64_decode(const string& input) {
    static const char* base64_chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";

    std::vector<unsigned char> result;
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
std::pair<mpz_class, mpz_class> read_pem_file(const string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Failed to open file: " << filename << std::endl;
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
    std::vector<unsigned char> binary_data = base64_decode(base64_content);
    
    // Parse custom format: [n_size][n_bytes][key_size][key_bytes]
    if (binary_data.size() < 8) {
        std::cerr << "Invalid key file format" << std::endl;
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