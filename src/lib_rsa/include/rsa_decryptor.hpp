#pragma once

#include <cstdint>

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
        RSADecryptor(const std::uint8_t bit_width = 31);

        const PublicKey& get_public_key() const;

        const std::uint64_t decrypt_cipher(const std::uint64_t c) const;
        

    private:
        KeyGenerationResult try_create_key_pair();

        std::uint64_t calc_phi() const;

        std::uint64_t calc_n() const;

        std::uint64_t choose_public_key();
};