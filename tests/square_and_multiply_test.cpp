#include <gtest/gtest.h>
#include "square_and_multiply.hpp"

// Potentiating with 0 should always result in 1
TEST(SNMTests, SNM_Exp_0) {
    const auto expected = 1;
    const auto actual = square_and_multiply(100, 0, 1000);
    ASSERT_EQ(expected, actual);
}

// Potentiating with 1 should always return original number
TEST(SNMTests, SNM_Exp_1) {
    const auto expected = 100;
    const auto actual = square_and_multiply(100, 1, 1000);
    ASSERT_EQ(expected, actual);
}

// Having modul 1 should always return 0
TEST(SNMTests, SNM_Mod_1) {
    const auto expected = 0;
    const auto actual = square_and_multiply(100, 1, 1);
    ASSERT_EQ(expected, actual);
}

// Having modul 2 should always return 0 or 1
TEST(SNMTests, SNM_Mod_2) {
    const auto expected = 1;
    const auto actual = square_and_multiply(101, 3, 2);
    ASSERT_EQ(expected, actual);
}

// 100^2 mod 1000 should return 0
TEST(SNMTests, SNM_Exp_100_2_1000) {
    const auto expected = 0;
    const auto actual = square_and_multiply(100, 2, 1000);
    ASSERT_EQ(expected, actual);
}

// 3^17 mod 23 should return 489 (see Beutelspacher, Neumann, Schwarzpaul. Kryptografie in Theorie und Praxis: Mathematische Grundlagen für Internetsicherheit, Mobilfunk und Elektronisches Geld, p.120)
TEST(SNMTests, SNM_Exp_3_17_21) {
    const auto expected = 16;
    const auto actual = square_and_multiply(3, 17, 23);
    ASSERT_EQ(expected, actual);
}

// 3^218 mod 1000 should return 489 (see Hoffstein, Pipher, Silverman. An Introduction to Mathematical Cryptography, p.25)
TEST(SNMTests, SNM_Exp_3_218_1000) {
    const auto expected = 489;
    const auto actual = square_and_multiply(3, 218, 1000);
    ASSERT_EQ(expected, actual);
}

// 2748^13 mod 2^64 - 1 should return 11826121633397542203
TEST(SNMTests, SNM_Exp_2748_13_UINT64_MAX) {
    const auto expected = 11826121633397542203ULL;
    const auto actual = square_and_multiply(2748, 13, UINT64_MAX);
    ASSERT_EQ(expected, actual);
}