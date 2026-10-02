#include <string>
#include <cstdint>
#include <vector>

std::vector<std::uint32_t> rsa_encrypt_msg(const std::string& msg);
std::uint32_t rsa_encrypt_block(const std::uint32_t block, const std::uint64_t e, const std::uint64_t n);