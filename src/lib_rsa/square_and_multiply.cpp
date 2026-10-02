#include <cstdint>
#include <bit>

#ifdef DEBUG
#include <print>
#endif


static std::uint64_t mul_mod(std::uint64_t a, std::uint64_t b, std::uint64_t mod) {
    unsigned __int128 product = static_cast<unsigned __int128>(a) * b;
    return static_cast<std::uint64_t>(product % mod);
}


std::uint64_t square_and_multiply(const std::uint64_t num, const std::uint64_t e, const std::uint64_t n) {
    // TODO: Panic if e > n

    if (e == 0) return 1;
    if (e == 1) return num;

    // Find the highest bit in the exponent
    int width = std::bit_width(e);

#ifdef DEBUG
    std::print("Bit width is: {} in line {} of file {}", width, __LINE__, __FILE__);
#endif

    std::uint64_t bit_mask = 1ULL << (width - 1);

    // Main loop
    std::uint64_t result = 1;
    for (int i = 0; i < width; i++) {
        result = mul_mod(result, result, n);

        // If the next bit is set, then multiply
        if ((e & bit_mask) != 0) {
            result = mul_mod(result, num, n);
        }

        // Update the bit mask
        bit_mask >>= 1;
    }

    return result;
}