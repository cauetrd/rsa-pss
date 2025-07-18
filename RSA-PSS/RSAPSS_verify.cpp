#include "RSAPSS_verify.hpp"

bool read_sign_file(const std::string& sign_filename){
    std::ifstream file(sign_filename, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file: " << sign_filename << std::endl;
        return false;
    }

    std::string filename_base64;
    std::string algorithm_base64;
    std::string modulus_base64;
    std::string public_key_base64;
    std::string signature_base64;
    
    std::string line;
    while (std::getline(file, line)) {
        
        if (line.find("filename:") == 0) {
            filename_base64 = line.substr(9);
        } else if (line.find("algorithm:") == 0) {
            algorithm_base64 = line.substr(10);
        } else if (line.find("modulus:") == 0) {
            modulus_base64 = line.substr(8);
        } else if (line.find("public key:") == 0) {
            public_key_base64 = line.substr(11);
        } else if (line.find("signature:") == 0) {
            signature_base64 = line.substr(10);
        }
    }
    
    
    std::string original_filename = string_base64_decode(filename_base64);
    std::string algorithm = string_base64_decode(algorithm_base64);
    std::string modulus_hex = string_base64_decode(modulus_base64);
    std::string public_key_hex = string_base64_decode(public_key_base64);
    std::vector<unsigned char> signature_bytes = base64_decode(signature_base64);

    mpz_class modulus, public_key, signature;
    mpz_set_str(modulus.get_mpz_t(), modulus_hex.c_str(), 16);
    mpz_set_str(public_key.get_mpz_t(), public_key_hex.c_str(), 16);
    mpz_import(signature.get_mpz_t(), signature_bytes.size(), 1, 1, 0, 0, signature_bytes.data());

    if(algorithm != "RSA-PSS-SHA3-256") {
        std::cerr << "Unsupported algorithm: " << algorithm << std::endl;
        return false;
    }

    std::vector<unsigned char> M = read_file_bytes(original_filename);
    if (M.empty()) {
        std::cerr << "Failed to read the file content: " << original_filename << std::endl;
        return false;
    }

    return RSAPSS_verify(M, signature, modulus, public_key);
}

bool RSAPSS_verify(const std::vector<unsigned char>& M, mpz_class s, mpz_class n, mpz_class e) {
    
    std::cout << "\n=== INICIANDO VERIFICAÇÃO RSA-PSS ===" << std::endl;
    
    std::cout << "n: " << n << std::endl;
    std::cout << "e: " << e << std::endl;
    std::cout << "s: " << s << std::endl;

    int modulus_bits = mpz_sizeinbase(n.get_mpz_t(), 2);
    
    int expected_size = (modulus_bits + 7) / 8;

    int signature_bits = mpz_sizeinbase(s.get_mpz_t(), 2);
    int signature_size = (signature_bits + 7) / 8;

    if(signature_size != expected_size) {
        std::cerr << "Tamanho da assinatura incorreto. Esperado: " << expected_size 
                  << ", Recebido: " << signature_size << std::endl;
        return false;
    }

    std::cout << "Executando decifração RSA (s^e mod n)..." << std::endl;
    mpz_class m = rsa_decrypt(s, n, e);
    std::cout << "Mensagem decifrada (m): " << m << std::endl;

    std::cout << "Preparando EM (Encoded Message)..." << std::endl;
    std::vector<unsigned char> EM;
    int emBits = modulus_bits - 1;
    int emLen = (emBits + 7) / 8;
    
    EM.resize(emLen);
    std::fill(EM.begin(), EM.end(), 0);
    
    size_t mLen = (mpz_sizeinbase(m.get_mpz_t(), 2) + 7) / 8;
    mpz_export(EM.data() + (emLen - mLen), nullptr, 1, 1, 0, 0, m.get_mpz_t());
    
    std::cout << "EM preparado. Tamanho: " << EM.size() << " bytes" << std::endl;
    std::cout << "emBits: " << emBits << ", emLen: " << emLen << std::endl;

    std::cout << "Executando verificação PSS..." << std::endl;
    bool isValid = PSS_verify(M, EM, emBits);

    if(isValid) {
        std::cout << "✓ ASSINATURA VÁLIDA!" << std::endl;
        return true;
    } else {
        std::cout << "✗ ASSINATURA INVÁLIDA!" << std::endl;
        return false;
    }
}