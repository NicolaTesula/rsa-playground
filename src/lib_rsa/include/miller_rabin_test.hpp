#pragma once

#include <cstdint>

enum class MillerRabinResult {
    IsComposite,
    IsLikelyPrime
};

MillerRabinResult miller_rabin_round(const std::uint32_t n, std::uint32_t a);
MillerRabinResult miller_rabin_test(const std::uint32_t n, const std::uint8_t num_rounds = 10);