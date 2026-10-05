#include <stdexcept>

#include "extended_euclidean_algorithm.hpp"

std::tuple<std::int64_t, std::int64_t, std::int64_t> extended_euclidean_algorithm(std::uint64_t a, std::uint64_t b) {
    // Efficient implementation after Hoffstein, Pipher, Silverman. An Introduction to Mathematical Cryptography, p.50
    // TODO: handle this differently?
    if (b == 0) throw std::invalid_argument("b cannot be 0 (Division by 0 error)!");

    // 1.
    std::int64_t u = 1, g = a, x = 0, y = b;

    // 2. (start, flipped)
    while (y != 0) {
        // 3.
        std::int64_t t = g % y;
        std::int64_t q = g / y;
        
        // 4.
        std::int64_t s = u - q * x;
        
        // 5.
        u = x, g = y;
        
        // 6.
        x = s, y = t;
    }

    // 2. (end)
    std::int64_t v = static_cast<std::int64_t>(
        (g - static_cast<__int128>(a) * u) / static_cast<__int128>(b) // avoid overflow
    );
    return {g, u, v}; // g auto casted to u64
}