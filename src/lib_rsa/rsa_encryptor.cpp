#include <stdexcept>
#include <bit>

#include "rsa_encryptor.hpp"
#include "square_and_multiply.hpp"
#include "block_encoding.hpp"


RSAEncryptor::RSAEncryptor() : m_pk{0,0} {}

RSAEncryptor::RSAEncryptor(const PublicKey& pk) : m_pk(pk) {}

PublicKey RSAEncryptor::get_public_key() const {
    return m_pk;
}

void RSAEncryptor::set_public_key(const PublicKey& pk) {
    m_pk = pk;
}

std::uint64_t RSAEncryptor::encrypt_cipher(std::uint64_t m) const {
    // m must be smaller than n to not lose information
    if (m >= m_pk.n) throw std::invalid_argument("m must be smallert than n!");

    return square_and_multiply(m, m_pk.e, m_pk.n);
}

std::vector<std::uint64_t> RSAEncryptor::encrypt_msg(const std::string& msg) const {
    if (m_pk.n == 0) throw std::logic_error("No valid public key! Please set one first!");

    // Split the message into blocks numerically smaller than n
    int module_bit_width = std::bit_width(m_pk.n);
    int block_bit_width = module_bit_width - 1;
    // TODO: change the block bit width logic. latest when implementing probabilistic encryption
    // (pass 2nd argument "bits_for_rnd" or smth like that?)


    std::vector<std::uint64_t> blocks = string_to_blocks(msg, block_bit_width);

    // Encrypt each block and gather the results in a new vector
    std::vector<std::uint64_t> encrypted;
    encrypted.reserve(blocks.size());
    for (std::uint64_t block : blocks) {
        encrypted.push_back(encrypt_cipher(block));
    }

    return encrypted;
}
