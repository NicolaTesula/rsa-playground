#include <gtest/gtest.h>
#include "extended_euclidean_algorithm.hpp"

typedef std::uint64_t u64;
typedef std::int64_t i64;

// Having b == 0 should always throw an exception
TEST(EEATests, b_0) {
    EXPECT_THROW({
        try {
            extended_euclidean_algorithm(42, 0);
        } catch(const std::invalid_argument& e) {
            // This tests that the exception has the correct message
            EXPECT_STREQ("b cannot be 0 (Division by 0 error)!", e.what());
            throw;
        }
    }, std::invalid_argument);
}

// gcd(527, 1258) should be 17, u should be -31 v should be 13
TEST(EEATests, a_527_b_1258) {
    const auto expected = std::tuple<u64, i64, i64>(17, -31, 13);
    const auto actual = extended_euclidean_algorithm(527, 1258);
    EXPECT_EQ(expected, actual) << "EEA failed on a = 527, b = 1258";
}

// gcd(163961, 167181) should be 7, u should be 4517 v should be -4430
TEST(EEATests, a_163961_b_167181) {
    const auto expected = std::tuple<u64, i64, i64>(7, 4517, -4430);
    const auto actual = extended_euclidean_algorithm(163961, 167181);
    EXPECT_EQ(expected, actual) << "EEA failed on a = 163961, b = 167181";
}

// gcd(3892392, 239847) should be 3, u should be -36515 v should be 592589
TEST(EEATests, a_3892392_b_239847) {
    const auto expected = std::tuple<u64, i64, i64>(3, -36515, 592589);
    const auto actual = extended_euclidean_algorithm(3892392, 239847);
    EXPECT_EQ(expected, actual) << "EEA failed on a = 3892392, b = 239847";
}