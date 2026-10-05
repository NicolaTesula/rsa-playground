#include <gtest/gtest.h>
#include "square_and_multiply.hpp"

// Potentiating with 0 should always result in 1
TEST(SNMTests, Exp_0) {
    const auto expected = 1;
    const auto actual = square_and_multiply(100, 0, 1000);
    EXPECT_EQ(expected, actual);
}

// Potentiating with 1 should always return original number
TEST(SNMTests, Exp_1) {
    const auto expected = 100;
    const auto actual = square_and_multiply(100, 1, 1000);
    EXPECT_EQ(expected, actual);
}

// Having modul 0 should always throw an exception
TEST(SNMTests, Mod_0) {
    EXPECT_THROW({
        try {
            square_and_multiply(42, 5, 0);
        } catch(const std::invalid_argument& e) {
            // This tests that the exception has the correct message
            EXPECT_STREQ("Modul cannot be 0 (Division by 0 error)!", e.what());
            throw;
        }
    }, std::invalid_argument);
}

// Having modul 1 should always return 0
TEST(SNMTests, Mod_1) {
    const auto expected = 0;
    const auto actual = square_and_multiply(100, 1, 1);
    EXPECT_EQ(expected, actual);
}

// Having modul 2 should always return 0 with even number
TEST(SNMTests, Mod_2_Even) {
    const auto expected = 0;
    const auto actual = square_and_multiply(100, 3, 2);
    EXPECT_EQ(expected, actual);
}

// Having modul 2 should always return 1 with odd number
TEST(SNMTestQ, Mod_2_Odd) {
    const auto expected = 1;
    const auto actual = square_and_multiply(101, 3, 2);
    EXPECT_EQ(expected, actual);
}

// 100^2 mod 1000 should return 0
TEST(SNMTests, Base_100_Exp_2_N_1000) {
    const auto expected = 0;
    const auto actual = square_and_multiply(100, 2, 1000);
    EXPECT_EQ(expected, actual);
}

// 3^17 mod 23 should return 489 (see Beutelspacher, Neumann, Schwarzpaul. Kryptografie in Theorie und Praxis: Mathematische Grundlagen für Internetsicherheit, Mobilfunk und Elektronisches Geld, p.120)
TEST(SNMTests, Base_3_Exp_17_N_21) {
    const auto expected = 16;
    const auto actual = square_and_multiply(3, 17, 23);
    EXPECT_EQ(expected, actual);
}

// 3^218 mod 1000 should return 489 (see Hoffstein, Pipher, Silverman. An Introduction to Mathematical Cryptography, p.25)
TEST(SNMTests, Base_3_Exp_218_N_1000) {
    const auto expected = 489;
    const auto actual = square_and_multiply(3, 218, 1000);
    EXPECT_EQ(expected, actual);
}

// 2748^13 mod 2^64 - 1 should return 11826121633397542203
TEST(SNMTests, Base_2748_Exp_13_N_UINT64_MAX) {
    const auto expected = 11826121633397542203ULL;
    const auto actual = square_and_multiply(2748, 13, UINT64_MAX);
    EXPECT_EQ(expected, actual);
}