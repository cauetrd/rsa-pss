#include "RSAPSS_sign.hpp"


bool generate_sign_txt(const std::string& filename, const std::string& signature_base64) {
    std::string public_key_file = "public_key.pem";
    std::pair<mpz_class, mpz_class> public_key = read_pem_file(public_key_file);

    std::string filename_base64 = base64_encode(reinterpret_cast<const unsigned char*>(filename.c_str()), filename.size());
    std::string modulus_base64 = base64_encode(reinterpret_cast<const unsigned char*>(public_key.first.get_str(16).c_str()), public_key.first.get_str(16).size());
    std::string public_key_base64 = base64_encode(reinterpret_cast<const unsigned char*>(public_key.second.get_str(16).c_str()), public_key.second.get_str(16).size());
    std::string algorithm = "RSA-PSS-SHA3-256";
    std::string algorithm_base64 = base64_encode(reinterpret_cast<const unsigned char*>(algorithm.c_str()), algorithm.size());

    std::string sign_data = "filename:" + filename_base64 + "\n" +
                            "algorithm:" + algorithm_base64 + "\n" +
                            "modulus:" + modulus_base64+ "\n" +
                            "public key:" + public_key_base64 + "\n" +
                            "signature:" + signature_base64 + "\n";


    std::string sign_filename = filename + ".sign";
    std::ofstream sign_file(sign_filename, std::ios::binary);
    if (!sign_file) {
        std::cerr << "Failed to open file for writing: " << sign_filename << std::endl;
        return false;
    }

    size_t line_length = 64; // Maximum line length for Base64 encoding

    sign_file << sign_data;
    
    sign_file.close();
    return true;
}

std::string RSAPSS_sign(std::vector<unsigned char>& M){
    std::string private_key_file = "private_key.pem";

    std::pair<mpz_class, mpz_class> private_key = read_pem_file(private_key_file);
    mpz_class n = private_key.first;
    mpz_class d = private_key.second;

    int modulus_bits =  mpz_sizeinbase(n.get_mpz_t(),2); // Get the number of bits in key
    int emBits = modulus_bits - 1; // For RSA-PSS, emBits = modulus_bits - 1

    std::cout<< "Gerando EM para assinatura..." << std::endl;
    std::vector<unsigned char> EM = PSS_encode(M, emBits); // Encode the message M into EM

    mpz_class m;
    mpz_import(m.get_mpz_t(), EM.size(), 1, 1, 0, 0, EM.data()); // Convert EM from byte vector to mpz_class

    if(m < 0 or m >=n){
        std::cerr << "Message is out of range for RSA encryption." << std::endl;
        return {}; // Return empty vector if message is invalid
    }

    std::cout << "Assinando mensagem em RSA com" << std::endl;
    std::cout << "n: " << n << std::endl;
    std::cout << "d: " << d << std::endl;
    
    mpz_class s = rsa_encrypt(m, n, d);

    int signatureLen = (modulus_bits + 7) / 8; 

    std::cout << "Tamanho da assinatura: " << signatureLen << " bytes" << std::endl;
    std::cout << "Assinatura obtida: "<< s << std::endl;
    
    std::vector<unsigned char> signature_bytes(signatureLen, 0);
    size_t s_size = (mpz_sizeinbase(s.get_mpz_t(), 2) + 7) / 8;
    mpz_export(signature_bytes.data() + (signatureLen - s_size), nullptr, 1, 1, 0, 0, s.get_mpz_t());
        
    std::string signature_b64 = base64_encode(signature_bytes.data(), signature_bytes.size());
    
    return signature_b64; 
}   