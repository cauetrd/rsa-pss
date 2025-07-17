#ifndef HASHFILE_HPP
#define HASHFILE_HPP

#include "Keccak.h"
#include <iostream>
#include <iomanip>
#include <cstring>
#include <vector>
#include <fstream>
#include <gmpxx.h>

std::vector<unsigned char> read_file_bytes(const std::string &filename);

std::vector<unsigned char> hash_file_sha3_256(const std::string &filename);

std::vector<unsigned char> hash_bytes_sha3_256(const std::vector<unsigned char> &data);

#endif

