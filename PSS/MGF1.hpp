#ifndef MGF1_HPP
#define MGF1_HPP

#include <vector>
#include "../hash/hashFile.hpp" 

std::vector<unsigned char> mgf1(const std::vector<unsigned char>& seed, size_t maskLen);

#endif // MGF1_HPP

