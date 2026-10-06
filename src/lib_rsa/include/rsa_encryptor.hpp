#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "public_key.hpp"

class RSAEncryptor {
    private:
        PublicKey m_pk;

    public:
        RSAEncryptor(const PublicKey& pk);

    private:
        std::uint64_t encrypt_cipher(std::uint64_t m);
        std::vector<std::uint64_t> encrypt_msg(const std::string& msg) const;
};