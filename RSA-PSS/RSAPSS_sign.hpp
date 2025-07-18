#ifndef RSAPSS_SIGN_HPP
#define RSAPSS_SIGN_HPP
 
#include <vector>
#include <cstring>
#include "gmpxx.h"
#include "../RSA/RSA-encrypt.hpp"
#include "../PSS/PSS_encoding.hpp"
#include "../PEM/PEM-read.hpp"
#include "../PEM/PEM-write.hpp"

std::string RSAPSS_sign(std::vector<unsigned char>& M);

#endif