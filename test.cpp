#include <iostream>
#include <cstring>
#include "RSA-PSS/RSAPSS_sign.hpp"
#include "RSA-PSS/RSAPSS_verify.hpp"
#include "hash/hashFile.hpp"
#include "key_gen/key_gen.hpp"

void sign(){
    std::string filename;

    std::cout << "Digite o nome do arquivo com extensão a ser assinado (Ex.: vascao.png): " << std::endl;
    std:: cin >> filename;

    // Read the file content
    std::vector<unsigned char> file_content = read_file_bytes(filename);
    if (file_content.empty()) {
        std::cerr << "Erro ao ler o arquivo: " << filename << std::endl;
        return;
    }
    // Sign the file content
    std::string signature = RSAPSS_sign(file_content);

    std::cout << "Assinatura gerada com sucesso!" << std::endl;
    std::cout << "Escrevendo assinatura no arquivo: " << filename << ".sign ..." << std::endl;
    bool works = generate_sign_txt(filename, signature);
    if(works){
        std::cout << "Assinatura escrita com sucesso!" << std::endl;
    } else {
        std::cerr << "Erro ao escrever assinatura no arquivo." << std::endl;
        return;
    }
    return;
}

void verify(){
    std::string sign_filename;

    std::cout << "Digite o nome do arquivo de assinatura (Ex.: vascao.png.sign): " << std::endl;
    std:: cin >> sign_filename;

    if(!read_sign_file(sign_filename)){
        std::cerr << "Erro ao ler o arquivo de assinatura." << std::endl;
        return;
    }
    std::cout << "Assinatura verificada com sucesso!" << std::endl;
}

void generate_keys() {
    key_gen();
}

int main() {
    int choice;
    std::cout << "Escolha uma opção:" << std::endl;
    std::cout << "1. Assinar arquivo" << std::endl;
    std::cout << "2. Verificar assinatura" << std::endl;
    std::cout << "3. Gerar novas chaves RSA" << std::endl;
    std::cin >> choice;

    switch (choice) {
        case 1:
            sign();
            break;
        case 2:
            verify();
            break;
        case 3:
            generate_keys();
            break;
        default:
            std::cerr << "Opção inválida." << std::endl;
            return 1;
    }

    return 0;

}