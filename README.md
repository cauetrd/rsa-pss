
# RSA-PSS (PT-BR)

Uma implementação completa do esquema de assinatura digital RSA-PSS (Probabilistic Signature Scheme) com hash SHA-3 (Keccak) para o trabalho 3 da disciplina de Segurança Computacional.

## Visão Geral

Este projeto implementa um sistema de assinatura digital baseado em RSA-PSS que oferece:

- **Geração de chaves RSA** de 2048 bits com testes de primalidade Miller-Rabin
- **Assinatura digital** usando esquema PSS (Probabilistic Signature Scheme)
- **Verificação de assinatura** com validação completa do esquema PSS
- **Hash SHA-3 (Keccak)** para todas as operações criptográficas
- **Formato PEM** para armazenamento de chaves
- **Codificação Base64** para assinaturas e metadados

## Pré-requisitos

### Dependências Obrigatórias

1. **Compilador C++17**

   - GCC 7.0 ou superior
   - Suporte completo ao C++17

2. **GNU Multiple Precision Arithmetic Library (GMP)**

   - **Ubuntu/Debian:**
     ```bash
     sudo apt-get update
     sudo apt-get install libgmp-dev libgmpxx4ldbl
     ```
   - **CentOS/RHEL/Fedora:**
     ```bash
     sudo yum install gmp-devel
     # ou para versões mais recentes:
     sudo dnf install gmp-devel
     ```
   - **macOS:**
     ```bash
     brew install gmp
     ```
   - **Windows (MSYS2):**
     ```bash
     pacman -S mingw-w64-x86_64-gmp
     ```

3. **Make**
   - Disponível na maioria dos sistemas Unix/Linux
   - Para Windows, use MSYS2 ou WSL

### Verificação das Dependências

Para verificar se as dependências estão instaladas:

```bash
# Verificar GCC
gcc --version

# Verificar GMP
pkg-config --cflags --libs gmp

# Verificar Make
make --version
```

## Instalação e Compilação

### 1. Clone/Download do Projeto

```bash
git clone <repository-url>
cd trabalho3
```

### 2. Compilação

**Modo Release (Recomendado):**

```bash
make
```

**Modo Debug:**

```bash
make debug
```

**Limpeza:**

```bash
make clean
```

### 3. Verificação da Compilação

Se a compilação foi bem-sucedida, você deverá ver:

```
Linking run...
```

E o executável `run` será criado no diretório principal.

## Como Usar

### Execução do Programa

```bash
./run
```

### Interface do Usuário

O programa apresenta um menu interativo:

```
Escolha uma opção:
1. Assinar arquivo
2. Verificar assinatura
3. Gerar novas chaves RSA
```

### 1. Gerar Chaves RSA (Primeira Execução)

Antes de assinar qualquer arquivo, execute:

```bash
./run
# Escolha opção 3
```

**O que acontece:**

- Gera dois primos de 1024 bits (p e q)
- Usa Miller-Rabin com 40 iterações para testes de primalidade
- Calcula n = p × q e φ(n) = (p-1)(q-1)
- Usa e = 65537 (expoente público padrão)
- Calcula d = e^(-1) mod φ(n) (chave privada)
- Salva as chaves em formato PEM

**Arquivos gerados:**

- `public_key.pem` - Chave pública (n, e)
- `private_key.pem` - Chave privada (n, d)

**Tempo esperado:** 0,1-1,5 segundos dependendo do hardware

### 2. Assinar um Arquivo

```bash
./run
# Escolha opção 1
# Digite o nome do arquivo (ex: vascao.png)
```

**O que acontece:**

- Lê o arquivo especificado
- Calcula hash SHA-3-256 do conteúdo
- Aplica codificação PSS com salt aleatório
- Assina usando RSA com chave privada
- Gera arquivo `.sign` com metadados em Base64

**Exemplo:**

```
Digite o nome do arquivo com extensão a ser assinado (Ex.: vascao.png):
documento.pdf
```

**Arquivo gerado:**

- `documento.pdf.sign` - Contém assinatura e metadados

### 3. Verificar uma Assinatura

```bash
./run
# Escolha opção 2
# Digite o nome do arquivo .sign
```

**O que acontece:**

- Lê o arquivo de assinatura
- Extrai metadados (nome do arquivo, chave pública, assinatura)
- Lê o arquivo original
- Verifica a assinatura usando RSA e validação PSS
- Compara hashes para confirmar integridade

**Exemplo:**

```
Digite o nome do arquivo de assinatura (Ex.: vascao.png.sign):
documento.pdf.sign
```

## Estrutura dos Arquivos

### Arquivos de Chave (PEM)

```
-----BEGIN RSA PUBLIC KEY-----
<dados em Base64>
-----END RSA PUBLIC KEY-----
```

### Arquivos de Assinatura (.sign)

```
filename:ZG9jdW1lbnRvLnBkZg==
algorithm:UlNBLVBTUy1TSEEzLTI1Ng==
modulus:MjFiNzEzNjNjZjg5NmQ5OTk...
public key:MTAwMDE=
signature:ID2khvXlF5SSu7sez/v8t3QKvGup...
```

Todos os campos são codificados em Base64 para garantir compatibilidade.

## Resultados Esperados

### Geração de Chaves

```
PEM file written: public_key.pem
PEM file written: private_key.pem
Key generation took 0.56231 seconds.
```

### Assinatura Bem-sucedida

```
Assinatura gerada com sucesso!
Escrevendo assinatura no arquivo: documento.pdf.sign ...
Assinatura escrita com sucesso!
```

### Verificação Válida

```
=== INICIANDO VERIFICAÇÃO RSA-PSS ===
Executando decifração RSA (s^e mod n)...
Executando verificação PSS...
Consistent
✓ ASSINATURA VÁLIDA!
Assinatura verificada com sucesso!
```

### Verificação Inválida

```
=== INICIANDO VERIFICAÇÃO RSA-PSS ===
Executando decifração RSA (s^e mod n)...
Executando verificação PSS...
Inconsistent
✗ ASSINATURA INVÁLIDA!
Erro ao ler o arquivo de assinatura.
```

## Solução de Problemas

### Erro de Compilação: "gmp.h not found"

**Solução:** Instale a biblioteca GMP:

```bash
sudo apt-get install libgmp-dev libgmpxx4ldbl
```

### Erro: "Failed to open /dev/urandom"

**Causa:** Sistema não-Unix ou permissões insuficientes
**Solução:** Execute em ambiente Linux/Unix ou WSL

### Erro: "Failed to read enough bytes"

**Causa:** Fonte de entropia insuficiente
**Solução:** Aguarde alguns segundos e tente novamente

### Assinatura sempre inválida

**Verificações:**

1. Arquivo original não foi modificado
2. Arquivo .sign não foi corrompido
3. Chaves correspondem (mesma public_key.pem)

### Tempo de geração muito longo

**Normal:** 10-30 segundos em hardware moderno
**Lento:** >60 segundos indica hardware limitado ou alta carga do sistema

## Características Técnicas

### Algoritmos Implementados

- **RSA-PSS:** Esquema de assinatura probabilística
- **SHA3-256 (Keccak):** Hash function padrão FIPS 202
- **Miller-Rabin:** Teste de primalidade probabilística
- **MGF1:** Mask Generation Function baseada em SHA-3
- **Base64:** Codificação para compatibilidade

### Parâmetros de Segurança

- **Tamanho da chave:** 2048 bits
- **Expoente público:** 65537
- **Iterações Miller-Rabin:** 40
- **Tamanho do hash:** 256 bits (SHA-3)
- **Tamanho do salt:** 256 bits (igual ao hash)

### Compatibilidade

- **Sistemas:** Linux, macOS, Windows (WSL/MSYS2)
- **Compiladores:** GCC 7+, Clang 6+
- **Padrão:** C++17

## Limitações

- **Fonte de entropia:** Requer `/dev/urandom` (sistemas Unix)
- **Tamanho de arquivo:** Limitado pela memória disponível
- **Paralelização:** Não implementada (single-threaded)
- **Formato:** Apenas arquivos binários/texto

## Segurança

Este projeto implementa algoritmos criptográficos para fins educacionais. Para uso em produção, considere:

- Revisão por especialistas em criptografia
- Testes de penetração
- Auditoria de segurança
- Implementação de contramedidas contra ataques de canal lateral

## Autores

Cauê de Macedo Britto Trindade de Sousa - 231019003 &
Vinícius Da Silva Araújo - 221001981

Implementado para o Trabalho 3 da disciplina de Segurança Computacional - UnB.


# RSA-PSS Digital Signature Implementation (EN)

A complete implementation of RSA-PSS (Probabilistic Signature Scheme) digital signature system in C++, featuring key generation, file signing, and signature verification using SHA-3 hashing.

## 🔧 Prerequisites

### System Requirements

- **Operating System**: Unix-like environment (Linux, macOS, or WSL on Windows)
- **Compiler**: GCC with C++17 support
- **Build System**: Make utility

### Required Dependencies

#### 1. GMP Library (GNU Multiple Precision Arithmetic Library)

This is the only external dependency required for arbitrary precision arithmetic.

**Ubuntu/Debian:**

```bash
sudo apt-get update
sudo apt-get install libgmp-dev libgmpxx4ldbl
```

**CentOS/RHEL/Fedora:**

```bash
# CentOS/RHEL
sudo yum install gmp-devel gmp-c++

# Fedora
sudo dnf install gmp-devel gmp-c++
```

**macOS:**

```bash
brew install gmp
```

**Arch Linux:**

```bash
sudo pacman -S gmp
```

#### 2. Build Tools

```bash
# Ubuntu/Debian
sudo apt-get install build-essential make

# CentOS/RHEL
sudo yum groupinstall "Development Tools"

# Fedora
sudo dnf groupinstall "Development Tools"

# macOS (if not already installed)
xcode-select --install
```

## 🚀 Installation & Compilation

1. **Clone or download the project**
2. **Navigate to the project directory**

   ```bash
   cd trabalho3
   ```

3. **Compile the project**

   ```bash
   make
   ```

4. **Optional: Build in debug mode**

   ```bash
   make debug
   ```

5. **Clean build files (if needed)**
   ```bash
   make clean
   ```

## 📋 Usage

The program provides three main functionalities through an interactive menu:

### Running the Program

```bash
./run
```

### Program Menu Options

**1. Sign a File**

- Generates a digital signature for any file
- Creates a `.sign` file containing the signature and metadata
- Uses RSA-PSS with SHA-3 256-bit hashing

**2. Verify a Signature**

- Verifies the authenticity of a previously signed file
- Checks signature validity and file integrity
- Displays detailed verification process

**3. Generate New RSA Keys**

- Creates new 2048-bit RSA key pair
- Uses Miller-Rabin primality testing (40 iterations)
- Saves keys in PEM format (`public_key.pem` and `private_key.pem`)

## 🎯 Expected Behavior

### First Run - Key Generation

If no keys exist, generate them first:

```bash
./run
# Choose option 3: Generate new RSA keys
```

**Expected Output:**

```
Key generation took X.XXX seconds.
PEM file written: public_key.pem
PEM file written: private_key.pem
```

### Signing a File

```bash
./run
# Choose option 1: Sign a file
# Enter filename: vascao.png
```

**Expected Output:**

```
Gerando EM para assinatura...
Assinando mensagem em RSA com
n: [large number]
d: [large number]
Tamanho da assinatura: 256 bytes
Assinatura obtida: [signature number]
Assinatura gerada com sucesso!
Escrevendo assinatura no arquivo: vascao.png.sign ...
Assinatura escrita com sucesso!
```

### Verifying a Signature

```bash
./run
# Choose option 2: Verify signature
# Enter signature file: vascao.png.sign
```

**Expected Output:**

```
=== INICIANDO VERIFICAÇÃO RSA-PSS ===
n: [modulus]
e: [public exponent]
s: [signature]
Executando decifração RSA (s^e mod n)...
Mensagem decifrada (m): [decrypted message]
Preparando EM (Encoded Message)...
EM preparado. Tamanho: 255 bytes
emBits: 2047, emLen: 255
Executando verificação PSS...
Consistent
✓ ASSINATURA VÁLIDA!
Assinatura verificada com sucesso!
```

## 📁 Generated Files

### Key Files

- `public_key.pem`: RSA public key in PEM format
- `private_key.pem`: RSA private key in PEM format

### Signature Files

- `[filename].sign`: Contains signature and metadata in Base64 format

**Example signature file structure:**

```
filename:dmFzY2FvLnBuZw==
algorithm:UlNBLVBTUy1TSEEzLTI1Ng==
modulus:[base64 encoded modulus]
public key:[base64 encoded public exponent]
signature:[base64 encoded signature]
```

## 🔒 Security Features

- **2048-bit RSA keys** (exceeds 1024-bit minimum requirement)
- **Miller-Rabin primality testing** with 40 iterations
- **RSA-PSS probabilistic padding** for enhanced security
- **SHA-3 (Keccak) hashing** for cryptographic strength
- **Cryptographically secure random number generation** via `/dev/urandom`

## 🛠️ Technical Implementation

### Core Components

- **Key Generation**: Miller-Rabin primality testing, modular arithmetic
- **Digital Signature**: RSA-PSS encoding with SHA-3
- **File Processing**: Binary file reading and Base64 encoding
- **Verification**: Complete PSS verification with hash comparison

### Libraries Used

- **GMP**: Multiple precision arithmetic (only external dependency)
- **Custom SHA-3**: FIPS 202 compliant Keccak implementation
- **Custom Base64**: RFC 4648 compliant encoding/decoding

## 🔍 Troubleshooting

### Common Issues

**1. Compilation Error: "gmp.h not found"**

```bash
# Install GMP development headers
sudo apt-get install libgmp-dev
```

**2. Linking Error: "undefined reference to \__gmpz_\*"**

```bash
# Ensure GMP libraries are installed
sudo apt-get install libgmp-dev libgmpxx4ldbl
```

**3. Permission Error: "Failed to open /dev/urandom"**

```bash
# Check if running on Unix-like system
# Windows users should use WSL
```

**4. Build Error: "make: command not found"**

```bash
# Install build tools
sudo apt-get install build-essential
```

### Debug Mode

For troubleshooting, compile in debug mode:

```bash
make debug
./run
```

## 📊 Performance Expectations

- **Key Generation**: 0.1-1.5 seconds (depends on system)
- **File Signing**: Near-instantaneous for typical files
- **Signature Verification**: Near-instantaneous
- **Memory Usage**: Minimal (all operations done in-place)

## 🧪 Testing

The project includes a test image (`vascao.png`) for demonstration:

```bash
./run
# Choose option 1, enter: vascao.png
# Choose option 2, enter: vascao.png.sign
```

## 🏗️ Build System

The Makefile supports:

- **Release build**: `make` or `make release`
- **Debug build**: `make debug`
- **Clean**: `make clean`
- **Run tests**: `make test`

## 📄 License

This implementation is for educational purposes and demonstrates RSA-PSS digital signatures without relying on external cryptographic libraries (except GMP for arithmetic and Keccak for hash).

## 🔗 Standards Compliance

- **RSA-PSS**: PKCS #1 v2.1 compliant
- **Base64**: RFC 4648 compliant
- **PEM Format**: RFC 7468 compatible

## Authors

Cauê de Macedo Britto Trindade de Sousa &
Vinícius Da Silva Araújo
