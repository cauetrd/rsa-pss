#ifndef RSAPSS_VERIFY_HPP
#define RSAPSS_VERIFY_HPP

#include <vector>
#include <iostream>
#include <gmpxx.h>
#include "../RSA/RSA-decrypt.hpp"
#include "../PEM/PEM-read.hpp"
#include "../PSS/PSS_verification.hpp"

bool read_sign_file(const std::string& filename);
bool RSAPSS_verify(const std::vector<unsigned char>& M, mpz_class s, mpz_class n, mpz_class e);

#endif