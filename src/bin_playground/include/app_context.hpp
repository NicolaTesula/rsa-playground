#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "public_key.hpp"
#include "rsa_encryptor.hpp"
#include "rsa_decryptor.hpp"
#include "miller_rabin_test.hpp"

struct PrimesContext {
    int bit_width = 30;
    std::uint32_t p = 0;
    std::uint32_t q = 0;
};

struct EncryptContext {
    int encrypted_scroll_offset = 0;
    int selected_cipher_block = 0;
    std::string user_input;
    std::vector<std::uint64_t> encrypted;
    std::vector<std::string> encrypted_hex_blocks;
    std::optional<RSAEncryptor> encryptor;
};

struct DecryptContext {
    int decrypted_scroll_offset = 0;
    int selected_cipher_block = 0;
    bool use_encrypted = false;
    std::string user_input;
    std::optional<std::string> decrypted;
    std::optional<RSADecryptor> decryptor;
};

struct StatusContext {
    std::optional<std::string> status_content;
    std::optional<PublicKey> curr_public_key;
    std::optional<std::string> details_content;
};

struct PrimalityContext {
    int num_to_check;
    std::string user_input;
    std::optional<MillerRabinResult> test_result;
};

struct AppContext {
    PrimesContext primes_ctx;
    EncryptContext encrypt_ctx;
    DecryptContext decrypt_ctx;
    StatusContext status_ctx;
    PrimalityContext primality_ctx;
};