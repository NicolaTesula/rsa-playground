#include <cstdint>
#include <stdexcept>

#include "block_encoding.hpp"


std::vector<std::uint64_t> string_to_blocks(const std::string& str, const int block_bit_width) {
    if (block_bit_width > 64) throw std::invalid_argument("One block cannot be wider than 64 bit!");

    std::vector<std::uint64_t> blocks;
    // TODO: reserve the necessary space here

    std::uint64_t block = 0;
    int bits_in_block = 0; // Keep track of how many bits we already pushed into the block
    std::uint64_t bit = 0; // The extracted bit from str

    // Iterate over each character in the message
    for (char ch : str) {
        // Iterate over each bit in char
        for (int i = 7; i >= 0; i--) {
            // Retrieve the i-th bit
            bit = (ch >> i) & 0x01; // 1000 0000 -> 0000 0001 -> 0000 ... 0001
            
            // Push the bit into the block (from right) and update the counter
            block |= bit << (block_bit_width - bits_in_block - 1); // 1000 ... 0000 -> 1010 ... 0000
            bits_in_block++;

            // Check if we reached the block size limit
            if (bits_in_block == block_bit_width) {
                // Push the block into the vector and start a new one
                blocks.push_back(block);
                block = bits_in_block = 0;
            }
        }
    }

    // Process the last started block and put it into the vector as well
    if (bits_in_block > 0) {
        blocks.push_back(block);
    }

    return blocks;
}


std::string blocks_to_string(const std::vector<std::uint64_t>& blocks, const int block_bit_width) {
    if (block_bit_width > 64) throw std::invalid_argument("One block cannot be wider than 64 bit!");

    std::string str = ""; // The string to return
    std::uint8_t ch = 0; // The currently handled character
    std::uint64_t bit = 0; // The extracted bit from the block
    std::uint8_t bits_in_char = 0; // Tracks how many bits are in the current char
    char c = ' '; // Char to cast the uint into

    // Iterate over the blocks
    for (auto block : blocks) {
        // Iterate over the bits in block
        for (int i = block_bit_width - 1; i >= 0; i--) {
            // Retrieve the i-th bit
            bit = (block >> i) & 0x01;

            // Push the bit into the char (from right) and update the counter
            ch |= bit << (7 - bits_in_char);
            bits_in_char++;

            // Check if we reached the block size limit
            if (bits_in_char == 8) {
                // Push the char into the string and start a new one
                c = static_cast<char>(ch);

                // Check if that was the last character in the string
                if (c == '\0') {
                    return str; // Avoid filling in more '\0' characters
                }
                
                str.push_back(c);
                ch = bits_in_char = 0;
            }
        }
    }

    return str;
}