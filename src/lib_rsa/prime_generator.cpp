#include <stdexcept>
#include <random>

#include "prime_generator.hpp"
#include "miller_rabin_test.hpp"

std::tuple<std::uint32_t, std::uint32_t> generate_primes(const std::uint8_t bit_width, LogCallback log) {
    if (bit_width > 31) throw std::invalid_argument("Higher bit width primes not supported!");

    // Initialize RNG
    static std::mt19937 gen(std::random_device{}()); // Get 64 bit mersenne twister and seed the generator
    std::uint32_t lower_bound = 1ULL << (bit_width - 1); // by default: 2^30
    std::uint32_t upper_bound = (1ULL << bit_width) - 1; // by default: 2^30 - 1
    std::uniform_int_distribution<std::uint32_t> dist(lower_bound, upper_bound); // Defines the range

    // Pick 2 random numbers to check for primality
    std::uint32_t p = dist(gen);
    std::uint32_t q = dist(gen);

    // Test both numbers with miller rabin
    MillerRabinResult is_p_prime = miller_rabin_test(p);
    while (is_p_prime == MillerRabinResult::IsComposite) {
        // Pick new number p and test again
        p = dist(gen);
        is_p_prime = miller_rabin_test(p);
    }
    MillerRabinResult is_q_prime = miller_rabin_test(q);
    while (is_q_prime == MillerRabinResult::IsComposite) {
        // Pick new number q and test again
        q = dist(gen);
        is_q_prime = miller_rabin_test(q);
    }
    
    return {p, q};
}