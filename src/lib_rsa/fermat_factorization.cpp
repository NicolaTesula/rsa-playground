#include <stdexcept>
#include <cmath>
#include <numeric>

#include "fermat_factorization.hpp"

// NOTE: This describes the simple version described in Hoffstein, Pipher, Silverman. An Introduction to Mathematical Cryptography, p.141 f. and does not include relation building
// TODO: overflow checks
PrimeFactors fermat_factorization(const std::uint64_t n) {
    // Early return
    if (n < 4) {
        throw std::invalid_argument("n must be a composite integer greater than 1!");
    }

    // Catch moduli in the form 2 * q
    if (n % 2 == 0) {
        return {2, n / 2};
    }

    // Iterate over multiples of n
    for (int k = 1; k < 100; k++) {
        // If k == 2, 2n+b^2 cannot be a square
        if (k == 2) continue;

        // Iterate over all b and check if they make n + b^2 a square
        for (std::uint64_t b = 1; b < (1ULL << 24); b++) {
            std::uint64_t possible_square = k * n + b * b;
            
            // Calculate the root as float and cut off / round down
            auto root = static_cast<std::uint64_t>(std::sqrt(possible_square));
            
            // If the square of the root still gives the possible square, we found our primes
            if (root * root == possible_square) {
                std::uint64_t a = root;

                // For multiples of n we need to check gcd but for k = 1 we can immediately return
                if (k == 1) {
                    return {a - b, a + b};
                } else {
                    std::uint64_t p = std::gcd(n, a + b);
                    std::uint64_t q = std::gcd(n, a - b);

                    // There is still a small chance, that the gcd didn't retrieve the prime factors
                    // So let's check
                    if (p * q != n) {
                        continue;
                    } else {
                        return {p, q};
                    }
                }
            }
        }
    }

    throw std::range_error("Fermat could not find a prime factor within the configured bound!");
}