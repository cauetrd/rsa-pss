#ifndef RSAPSS_VERIFY_HPP
#define RSAPSS_VERIFY_HPP

#include <vector>
#include <iostream>
#include <gmpxx.h>
#include "../RSA/RSA-decrypt.hpp"
#include "../PEM/PEM-read.hpp"
#include "../PSS/PSS_verification.hpp"

bool RSAPSS_verify(const std::vector<unsigned char>& M, const std::string& signatureBase64);

#endif