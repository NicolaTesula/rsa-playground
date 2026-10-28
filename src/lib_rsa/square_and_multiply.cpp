#include <cstdint>
#include <bit>
#include <stdexcept>

#ifdef DEBUG
#include <iostream>
#endif

/**
 * @brief Util function for circumventing 64 bit integer overflow when multiplying two uint32s
 * 
 * @param a 
 * @param b 
 * @param mod 
 * @return std::uint64_t 
 */
std::uint64_t mul_mod(std::uint64_t a, std::uint64_t b, std::uint64_t mod) {
    unsigned __int128 product = static_cast<unsigned __int128>(a) * b;
    return static_cast<std::uint64_t>(product % mod);
}

/**
 * @brief Calculates the result of modular powers
 * 
 * @param num the base
 * @param e the exponent
 * @param n the modul. Must **not** be 0
 * @return std::uint64_t 
 */
std::uint64_t square_and_multiply(const std::uint64_t base, const std::uint64_t exp, const std::uint64_t module) {
    // Panic if n is 0
    if (module == 0) throw std::invalid_argument("Modul cannot be 0 (Division by 0 error)!");

    // Base cases
    if (exp == 0) return 1;
    if (exp == 1) return base % module;

    // Find the highest bit in the exponent // TODO worth it?
    int width = std::bit_width(exp);

    // Use a mask to extract each exponent bit
    std::uint64_t bit_mask = 1ULL << (width - 1);

    // Main loop
    std::uint64_t result = 1;
    for (int i = 0; i < width; i++) {
        // Always square
        result = mul_mod(result, result, module);

        // If the current bit is set, multiply also
        if ((exp & bit_mask) != 0) {
            result = mul_mod(result, base, module);
        }

        // Update the bit mask
        bit_mask >>= 1;
    }

    return result;
}