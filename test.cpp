#include <iostream>
#include <cstring>
#include "RSA-PSS/RSAPSS_sign.hpp"
#include "RSA-PSS/RSAPSS_verify.hpp"
#include "hash/hashFile.hpp"


int main() {
    std::string filename;

    std::cout << "Digite o nome do arquivo com extensão a ser assinado (Ex.: vascao.png): " << std::endl;
    std:: cin >> filename;

    // Read the file content
    std::vector<unsigned char> file_content = read_file_bytes(filename);
    if (file_content.empty()) {
        std::cerr << "Erro ao ler o arquivo: " << filename << std::endl;
        return 0;
    }
    // Sign the file content
    std::vector<unsigned char> signature = RSAPSS_sign(file_content);

    std::cout << "Assinatura gerada com sucesso!" << std::endl;
    std::cout << "Assinatura: ";
    for (unsigned char byte : signature) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)byte;
    }
    
    std::cout << std::dec << std::endl;

    // Verify the signature
    std::cout << "Verificando a assinatura..." << std::endl;
    bool is_valid = RSAPSS_verify(file_content, signature);
    if (is_valid) {
        std::cout << "A assinatura é válida." << std::endl;
    } else {
        std::cout << "A assinatura é inválida." << std::endl;
    }

    return 0;
}