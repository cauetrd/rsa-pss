#ifndef SALT_GEN_HPP
#define SALT_GEN_HPP

#include <iostream>
#include <fstream>
#include <vector>

std::vector<unsigned char> generate_salt(size_t salt_length);

#endif