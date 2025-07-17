#include "hashFile.hpp"
#include "RSA-encrypt.hpp"
#include "RSA-decrypt.hpp"

#include <iostream>
#include <cstring>



int main() {
    std::string filename;

    std::cout << "Digite o nome do arquivo com extensão a ser assinado (Ex.: vascao.png): " << std::endl;
    std:: cin >> filename;

    mpz_class hash = hash_file_sha3_256(filename);

    mpz_class encrypted_hash = rsa_encrypt(hash);

    std::cout << "Digital signature: " << encrypted_hash << std::endl;

    mpz_class decrypted_hash = rsa_decrypt(encrypted_hash);

    std::cout << "Original hash: " << hash << std::endl;

    std::cout << "Decrypted hash: " << decrypted_hash << std::endl;
    
    if (hash == decrypted_hash) {
        std::cout << "Signature verification: SUCCESS" << std::endl;
    } else {
        std::cout << "Signature verification: FAILED" << std::endl;
    }

    return 0;
}