#ifndef PSS_ENCODING_HPP
#define PSS_ENCODING_HPP

#include <iostream>
#include <vector>
#include "../hash/hashFile.hpp"
#include "MGF1.hpp"
#include "../Utils/utils.hpp"

std::vector<unsigned char> PSS_encode(const std::vector<unsigned char>& M, int emBits);

#endif