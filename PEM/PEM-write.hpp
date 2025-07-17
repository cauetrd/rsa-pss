#ifndef PEM_WRITE_HPP
#define PEM_WRITE_HPP

#include <iostream>
#include <cstring>
#include <gmpxx.h>
#include <vector>
#include <string>
#include <fstream>

std::string base64_encode(unsigned char const* buf, unsigned int bufLen);

std::string to_pem(const std::string& base64_data, const std::string& label);

void write_pem_file(const mpz_class& n, const mpz_class& key, const std::string& filename, const std::string& label);
#endif