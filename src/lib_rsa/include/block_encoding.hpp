#pragma once

#include <vector>
#include <string>

std::vector<std::uint64_t> string_to_blocks(const std::string& msg, const int block_bit_width = 56);
std::string blocks_to_string(const std::vector<std::uint64_t>& blocks, const int block_bit_width = 56);