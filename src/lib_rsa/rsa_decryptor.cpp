#include <cstdint>

#include "prime_generator.hpp"
#include "extended_euclidean_algorithm.hpp"

class RSADecryptor {
    public:
        struct PublicKey {
            std::uint64_t n; // The modul
            std::uint64_t e; // The exponent
        };

        typedef std::uint64_t PrivateKey;

        
    private:
        std::uint32_t m_p, m_q; // The two prime numbers making up n
        PrivateKey m_sk;        // The private (secret) key
        PublicKey m_pk;         // The public key

        enum class KeyGenerationResult {
            Success,
            Failure
        };
        
    public:
        RSADecryptor(const std::uint8_t bit_width = 31) {
            // On default construction generate a random pair of large primes
            std::tie(m_p, m_q) = generate_primes(bit_width);
            
            // Try to find valid primes, a valid exponent and a matching inverse
            while (try_create_key_pair() != KeyGenerationResult::Success) {
                // Generate new primes and try again
                std::tie(m_p, m_q) = generate_primes(bit_width);
            }
        }

        const PublicKey& get_public_key() const {
            return m_pk;
        }

    private:
        KeyGenerationResult try_create_key_pair() {
            const std::uint64_t phi = calc_phi();
            const std::uint64_t n = calc_n();

            // Use e = 65537 for now // TODO: maybe reimplement for if e | phi
            const std::uint64_t e = 65537;

            // Check whether e is coprime to phi and if yes use u to calculate a private key
            const auto [g, u, v] = extended_euclidean_algorithm(e, phi);
            if (g != 1) {
                return KeyGenerationResult::Failure;
            }

            // Get u positive and as close to 0 as possible (eea guarantees u in (-phi, +phi))
            const std::uint64_t u_pos = (u > 0) ? u : u + phi;

            // Save the public and private keys
            m_sk = u_pos;
            m_pk = {n, e};

            return KeyGenerationResult::Success;
        }

        std::uint64_t calc_phi() const {
            return static_cast<std::uint64_t>(m_p - 1) * static_cast<std::uint64_t>(m_q - 1);
        }

        std::uint64_t calc_n() const {
            return static_cast<std::uint64_t>(m_p) * static_cast<std::uint64_t>(m_q);
        }
};