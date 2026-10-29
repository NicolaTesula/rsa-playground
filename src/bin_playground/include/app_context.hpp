#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "public_key.hpp"
#include "rsa_encryptor.hpp"
#include "rsa_decryptor.hpp"
#include "miller_rabin_test.hpp"
#include "indistinguishability_game.hpp"
#include "prime_factors.hpp"

struct PrimesContext {
    int bit_width = 18;
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
    std::uint32_t num_to_check = 0;
    std::string user_input;
    std::optional<MillerRabinResult> test_result;
};

struct FactorizeContext {
    std::string user_input;
    bool use_public_key_modulus = false;
    int selected_method = 0;
    std::optional<PrimeFactors> factors;
};

struct PrivateKeyContext {
    std::string public_exponent_input;
    std::optional<std::uint64_t> private_key;
};

struct IndistinguishabilityContext {
    std::string m0_input = "Attack at dawn";
    std::string m1_input = "Attack at dusk";
    std::vector<std::uint64_t> challenge_cipher;
    std::vector<std::string> challenge_hex_blocks;
    int scroll_offset = 0;

    int total_guesses = 0;
    int correct_guesses = 0;

    std::optional<bool> last_guess_correct;
    std::optional<int> last_actual_choice;
    std::optional<int> last_user_guess;

    std::optional<std::string> message_info;

    Oracle oracle;
};

struct AppContext {
    PrimesContext primes_ctx;
    EncryptContext encrypt_ctx;
    DecryptContext decrypt_ctx;
    StatusContext status_ctx;
    PrimalityContext primality_ctx;
    FactorizeContext factorize_ctx;
    PrivateKeyContext private_key_ctx;
    IndistinguishabilityContext indistinguishability_ctx;
};
