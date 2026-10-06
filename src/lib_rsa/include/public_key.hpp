#pragma once

#include <cstdint>

// The public key builds the bridge between decryptor and encryptor
struct PublicKey {
    std::uint64_t n; // The module
    std::uint64_t e; // The exponent
};