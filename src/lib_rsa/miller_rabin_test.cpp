#include <numeric>
#include <random>
#include <cstdint>
#include <stdexcept>

#ifdef DEBUG
#include <print>
#endif

#include "miller_rabin_test.hpp"
#include "square_and_multiply.hpp"

// n is the number to check
// a is the potential witness
MillerRabinResult miller_rabin_round(const std::uint32_t n, std::uint32_t a) {
    if (a == 0) throw std::invalid_argument("0 is no valid candidate for a witness!");
    if (a == 1) throw std::invalid_argument("1 is no valid candidate for a witness!");
	if (a > n) throw std::invalid_argument("Base larger than n could lead to wrong decision!");

    if (n == 2 || n == 3) {
        return MillerRabinResult::IsLikelyPrime;
    }
    
    // Can return early if n is even or a and n share a factor
    const int gcd = std::gcd<std::uint32_t, std::uint32_t>(a, n);
	if (n % 2 == 0 || 1 < gcd && gcd < n) {
		return MillerRabinResult::IsComposite;
	}
	
	// Calculate k and q in $n-1 = 2^k \cdot q$
	std::uint8_t k = 0;
    std::uint32_t m = n - 1;
    while (m % 2 == 0) {
        m /= 2;
        k++;
    }
    const std::uint32_t q = (n - 1) / (1ULL << k);
	
	// Set a = a^q (mod n)
	a = square_and_multiply(a, q, n);
	
	// If first number is 1 then every power of a will be 1
	if (a % n == 1) {
		return MillerRabinResult::IsLikelyPrime;
	}
	
	for (int i = 0; i < k; i++) {
		// If some number is n-1 then next squares will be 1
		if (a % n == n-1) {
			return MillerRabinResult::IsLikelyPrime;
		}
		a = static_cast<std::uint32_t>(
            (static_cast<std::uint64_t>(a) * a) % n
        );
	}
	
    // No number in sequence was -1, thus n cannot be prime
	return MillerRabinResult::IsComposite;
}

// n is the number to check
// num_rounds is the number of potential witnesses to try (by default = 10)
MillerRabinResult miller_rabin_test(const std::uint32_t n, const std::uint8_t num_rounds) {
    if (n == 0) throw std::invalid_argument("0 is neither prime nor composite!");
	if (n == 1) throw std::invalid_argument("1 is neither prime nor composite!");
    if (num_rounds == 0) throw std::invalid_argument("Cannot decide with no round. Please use at least 10 rounds!");
    if (n == 2 || n == 3) return MillerRabinResult::IsLikelyPrime;
    if (n % 2 == 0) return MillerRabinResult::IsComposite;

    // Initialize RNG
    static std::mt19937 gen(std::random_device{}()); // Get 64 bit mersenne twister and seed the generator
    std::uniform_int_distribution<std::uint32_t> dist(2, n - 2);

	for (int i = 0; i < num_rounds; i++) {
		// Pick random number between 2 and n-1 as witness
		std::uint64_t a = dist(gen);
		
        // If we found n to be composite, return immediately
		MillerRabinResult round_result = miller_rabin_round(n, a);
		if (round_result == MillerRabinResult::IsComposite) {
#ifdef DEBUG
			std::print("Found witness {} for number {} in round {}\n", a, n, i);
#endif
			return round_result;
		}
	}
	
    // Else after <num_round> n is likely prime with probability ~2^{-num_rounds}
	return MillerRabinResult::IsLikelyPrime;
}