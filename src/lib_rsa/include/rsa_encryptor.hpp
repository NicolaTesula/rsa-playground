#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "public_key.hpp"

class RSAEncryptor {
#ifdef PRIVATE_TESTS
    friend class RSAEncryptorTest;
#endif

    private:
        PublicKey m_pk;

    public:
        RSAEncryptor();
        RSAEncryptor(const PublicKey& pk);
        std::vector<std::uint64_t> encrypt_msg(const std::string& msg) const;
        PublicKey get_public_key() const;
        void set_public_key(const PublicKey& pk);

    private:
        std::uint64_t encrypt_cipher(std::uint64_t m) const;
};