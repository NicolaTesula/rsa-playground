#pragma once

#include <cstdint>
#include <vector>
#include <string>

#include "public_key.hpp"

class RSADecryptor {
#ifdef PRIVATE_TESTS
    friend class RSADecryptorTest; // Grant the test executable access to private members
#endif

    public:
        typedef std::uint64_t PrivateKey;

    private:
        std::uint32_t m_p, m_q; // The two prime numbers making up n
        PrivateKey m_sk;        // The private (secret) key
        PublicKey m_pk;         // The public key

        enum class KeyGenerationResult {
            Success,
            Failure
        };
   
    public:
        RSADecryptor(const std::uint32_t p, const std::uint32_t q, const std::uint64_t e); // Constructor for tests mostly (unless you know your primes)
        RSADecryptor(const std::uint32_t p, const std::uint32_t q);
        RSADecryptor(const std::uint8_t bit_width = 31);
        const PublicKey get_public_key() const;
        PrivateKey get_private_key() const; // TODO: this is only intended for the TUI
        std::string decrypt_msg(const std::vector<std::uint64_t>& cipher) const;
        
    private:
        std::uint64_t decrypt_cipher(const std::uint64_t c) const;
        KeyGenerationResult try_create_key_pair();
        std::uint64_t calc_phi() const;
        std::uint64_t calc_n() const;
        std::uint64_t choose_public_key();
};