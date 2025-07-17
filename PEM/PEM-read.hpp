#ifndef PEM_READ_HPP 
#define PEM_READ_HPP

#include <iostream>
#include <cstring>
#include <gmpxx.h>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

std::vector<unsigned char> base64_decode(const string& input);

std::pair<mpz_class, mpz_class> read_pem_file(const string& filename);

#endif