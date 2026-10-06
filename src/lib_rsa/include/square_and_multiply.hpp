#pragma once

#include <cstdint>

std::uint64_t mul_mod(std::uint64_t a, std::uint64_t b, std::uint64_t mod);
std::uint64_t square_and_multiply(const std::uint64_t base, const std::uint64_t exp, const std::uint64_t module);