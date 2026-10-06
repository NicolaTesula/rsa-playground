#include <cmath>

#ifdef DEBUG
#include <print>
#endif

#include "rsa_encryptor.hpp"
#include "square_and_multiply.hpp"

RSAEncryptor::RSAEncryptor(const PublicKey& pk) : m_pk(pk) {}

std::uint64_t RSAEncryptor::encrypt_cipher(std::uint64_t m) {
    return {};
}

std::vector<std::uint64_t> RSAEncryptor::encrypt_msg(const std::string& msg) const {
    return {};
}


// TODO: Change block size from 32 bit to 56


/**
 * @brief Splits a message into 32 bit blocks and applies the encryption to each block
 * 
 * @param msg the plain text
 * @return std::vector<std::uint64_t> the cipher text
 */
std::vector<std::uint32_t> rsa_encrypt_msg(const std::string& msg) {
    // EXAMPLE: msg = "A7 0F 3C 76 | 67 BC 91 20 | 11 00"
#ifdef DEBUG
    std::print("{}", msg);
#endif

    // Vector to retun. Will hold the cipher text in the end
    std::vector<std::uint32_t> cipher{};
    cipher.reserve(std::ceil(msg.length() / 8));

    const std::uint8_t BLOCK_SIZE = 4; // 4 bytes per block

    // i is the index into msg
    for (int i = 0; i < msg.size(); i += BLOCK_SIZE) {
        std::uint32_t block = 0;

        // j is the byte index into the block starting from least significant byte
        // for (int j = 0; j < )
    }

    return cipher;
}

/**
 * @brief Encrypts a single 32 bit number
 * 
 * @param block the number to encrypt
 * @param n the modul
 * @return std::uint32_t the resulting cipher
 */
std::uint32_t rsa_encrypt_block(const std::uint32_t block, const std::uint64_t e, const std::uint64_t n) {
    // Exponent 

    if (block == 0) return 0;
    if (block == 1) return 1;

    std::uint32_t c = square_and_multiply(block, e, n);

    return 0;
}