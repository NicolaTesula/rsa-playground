#include <cstdint>
#include <numeric>
#include <random>

#include "rsa_decryptor.hpp"
#include "public_key.hpp"
#include "prime_generator.hpp"
#include "extended_euclidean_algorithm.hpp"
#include "square_and_multiply.hpp"


RSADecryptor::RSADecryptor(const std::uint8_t bit_width) {
    if (bit_width > 31) throw std::invalid_argument("31 bits is the max width supported right now!");

    // On default construction generate a random pair of large primes
    std::tie(m_p, m_q) = generate_primes(bit_width);
    
    // Try to find valid primes, a valid exponent and a matching inverse
    while (try_create_key_pair() != KeyGenerationResult::Success) {
        // Generate new primes and try again
        std::tie(m_p, m_q) = generate_primes(bit_width);
    }
}

const PublicKey& RSADecryptor::get_public_key() const {
    return m_pk;
}

const std::uint64_t RSADecryptor::decrypt_cipher(const std::uint64_t c) const {
    return square_and_multiply(c, m_sk, m_pk.n);
}

RSADecryptor::KeyGenerationResult RSADecryptor::try_create_key_pair() {
    const std::uint64_t phi = calc_phi();
    const std::uint64_t n = calc_n();

    // Find a public exponent with gcd(e, phi) = 1
    const std::uint64_t e = choose_public_key();

    // Check whether e is coprime to phi and if yes use u to calculate a private key
    const auto [g, u, v] = extended_euclidean_algorithm(e, phi);
    if (g != 1) {
        return KeyGenerationResult::Failure;
    }

    // Get u positive and as close to 0 as possible (EEA guarantees that u \in (-phi, +phi))
    const std::uint64_t u_pos = (u > 0) ? u : u + phi;

    // Save the public and private keys
    m_sk = u_pos;
    m_pk = {n, e};

    return KeyGenerationResult::Success;
}

std::uint64_t RSADecryptor::calc_phi() const {
    return static_cast<std::uint64_t>(m_p - 1) * static_cast<std::uint64_t>(m_q - 1);
}

std::uint64_t RSADecryptor::calc_n() const {
    return static_cast<std::uint64_t>(m_p) * static_cast<std::uint64_t>(m_q);
}

std::uint64_t RSADecryptor::choose_public_key() {
    // Just use 65537 for now
    std::uint64_t e = 65537;
    const std::uint64_t phi = calc_phi();

    // Initialize RNG
    static std::mt19937_64 gen(std::random_device{}()); // Get 64 bit mersenne twister and seed the generator
    std::uniform_int_distribution<std::uint64_t> dist(3, (phi - 1) / 2); // Defines the range

    // If e has a common factor with phi...
    while (std::gcd(e, phi) != 1) {
        // ...draw a different number from the distribution
        e = 2 * dist(gen) - 1; // Only draw uneven numbers in the first place
    }

    return e;
}