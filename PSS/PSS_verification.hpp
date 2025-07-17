#ifndef PSS_VERIFICATION_HPP
#define PSS_VERIFICATION_HPP

#include <vector>
#include <iostream>
#include "../hash/hashFile.hpp"
#include "../Utils/utils.hpp"
#include "MGF1.hpp"

bool PSS_verify(std::vector<unsigned char> M, std::vector<unsigned char> EM, int emBits);

#endif