#include <stdexcept>

#include "prime_generator.hpp"

std::tuple<std::uint32_t, std::uint32_t> generate_primes(const std::uint8_t bit_width = 31) {
    // TODO

    if (bit_width > 31) throw std::invalid_argument("Higher bit width primes not supported!");

    std::uint32_t p = 13;
    std::uint32_t q =  7;
    
    return {p, q};
}