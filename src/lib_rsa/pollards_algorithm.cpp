#include <numeric>
#include <stdexcept>

#include "pollards_algorithm.hpp"
#include "square_and_multiply.hpp"

PrimeFactors pollards_algorithm(const std::uint64_t n) {
    // Early return
    if (n < 4) {
        throw std::invalid_argument("n must be a composite integer greater than 1!");
    }

    // Catch moduli in the form 2 * q
    if (n % 2 == 0) {
        return {2, n / 2};
    }

    // Define computational limits for the algorithm
    constexpr std::uint64_t bound = 10000;
    constexpr std::uint64_t max_base = 10;

    // Iterate over bases from 2 to 10
    for (std::uint64_t base = 2; base <= max_base; base++) {
        std::uint64_t a = base % n; // Make sure a is in correct range
        std::uint64_t d = 0; // To store gcd(a - 1, n)

        // Then calculate a^(j!) = (a^(j-1!))^j
        for (int j = 2; j <= bound; j++) {
            a = square_and_multiply(a, j, n);
            d = std::gcd(a - 1, n);

            if (1 < d && d < n) {
                return {d, n / d};
            }

            // If a is a multiple of n, aka 0 mod n, then following powers will all be 0
            if (d == n) {
                break;
            }
        }
    }

    throw std::range_error("Pollard could not find a prime factor within the configured bound!");
}