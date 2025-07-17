#include "PEM-write.hpp"

static const std::string base64_chars =
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";

std::string base64_encode(unsigned char const* buf, unsigned int bufLen) {
  std::string ret;
  int i = 0;
  int j = 0;
  unsigned char char_array_3[3];
  unsigned char char_array_4[4];

  while (bufLen--) {
    char_array_3[i++] = *(buf++);
    if (i == 3) {
      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
      char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
      char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
      char_array_4[3] = char_array_3[2] & 0x3f;

      for(i = 0; (i <4) ; i++)
        ret += base64_chars[char_array_4[i]];
      i = 0;
    }
  }

  if (i)
  {
    for(j = i; j < 3; j++)
      char_array_3[j] = '\0';

    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
    char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
    char_array_4[3] = char_array_3[2] & 0x3f;

    for (j = 0; (j < i + 1); j++)
      ret += base64_chars[char_array_4[j]];

    while((i++ < 3))
      ret += '=';
  }

  return ret;
}

std::string to_pem(const std::string& base64_data, const std::string& label) {
    std::string pem = "-----BEGIN " + label + "-----\n";
    for(size_t pos = 0; pos < base64_data.size(); pos += 64) {
        pem += base64_data.substr(pos, 64) + "\n"; // 64 characters per line
    }
    pem += "-----END " + label + "-----\n";
    return pem;
}

void write_pem_file(const mpz_class& n, const mpz_class& key, const std::string& filename, const std::string& label) {
    // Calculate required buffer sizes
    size_t n_count = (mpz_sizeinbase(n.get_mpz_t(), 2) + 7) / 8; // Convert bits to bytes
    size_t key_count = (mpz_sizeinbase(key.get_mpz_t(), 2) + 7) / 8;
    
    // Allocate buffers
    std::vector<unsigned char> n_bytes(n_count);
    std::vector<unsigned char> key_bytes(key_count);

    // Export the numbers to byte arrays
    size_t actual_n_count = 0, actual_key_count = 0;
    mpz_export(n_bytes.data(), &actual_n_count, 1, 1, 0, 0, n.get_mpz_t());
    mpz_export(key_bytes.data(), &actual_key_count, 1, 1, 0, 0, key.get_mpz_t());
    
    // Resize vectors to actual sizes
    n_bytes.resize(actual_n_count);
    key_bytes.resize(actual_key_count);

    std::vector<unsigned char> combined_bytes;

    uint32_t n_size = n_bytes.size();
    uint32_t key_size = key_bytes.size();

    // Combine n and key into a single byte vector
    // structure: [n_size (4 bytes)][n_bytes][key_size (4 bytes)][key_bytes]
    combined_bytes.insert(combined_bytes.end(), reinterpret_cast<unsigned char*>(&n_size), reinterpret_cast<unsigned char*>(&n_size) + sizeof(n_size));
    combined_bytes.insert(combined_bytes.end(), n_bytes.begin(), n_bytes.end());
    combined_bytes.insert(combined_bytes.end(), reinterpret_cast<unsigned char*>(&key_size), reinterpret_cast<unsigned char*>(&key_size) + sizeof(key_size));
    combined_bytes.insert(combined_bytes.end(), key_bytes.begin(), key_bytes.end());

    std::string base64_data = base64_encode(combined_bytes.data(), combined_bytes.size());
    std::string pem_data = to_pem(base64_data, "RSA " + label + " KEY");


    // Write the PEM data to a file
    std::ofstream out(filename);
    if (!out) {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
        return;
    }
    out << pem_data;
    out.close();

    std::cout << "PEM file written: " << filename << std::endl;
}