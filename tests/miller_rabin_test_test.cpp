#include <gtest/gtest.h>

#include "miller_rabin_test.hpp"

// --------------- Miller-Rabin-Round Tests -------------------

// Having a == 0 should throw an exception
TEST(MRTTests, Round_a_0) {
    EXPECT_THROW({
        try {
            miller_rabin_round(42, 0);
        } catch(const std::invalid_argument& e) {
            // This tests that the exception has the correct message
            EXPECT_STREQ("0 is no valid candidate for a witness!", e.what());
            throw;
        }
    }, std::invalid_argument);
}

// Having a == 1 should throw an exception
TEST(MRTTests, Round_a_1) {
    EXPECT_THROW({
        try {
            miller_rabin_round(42, 1);
        } catch(const std::invalid_argument& e) {
            // This tests that the exception has the correct message
            EXPECT_STREQ("1 is no valid candidate for a witness!", e.what());
            throw;
        }
    }, std::invalid_argument);
}

// Disallow numbers a larger than n
TEST(MRTTests, Round_n_13_a_26) {
    EXPECT_THROW({
        try {
            miller_rabin_round(13, 26);
        } catch(const std::invalid_argument& e) {
            // This tests that the exception has the correct message
            EXPECT_STREQ("Base larger than n could lead to wrong decision!", e.what());
            throw;
        }
    }, std::invalid_argument);
}

// gcd(7, 42) = 7, so we expect IsComposite
TEST(MRTTests, Round_n_42_a_7) {
    const auto expected = MillerRabinResult::IsComposite;
    const auto actual = miller_rabin_round(42, 7);
    EXPECT_EQ(expected, actual);
}

// 65537 is prime, so we expect IsLikelyPrime
TEST(MRTTests, Round_n_65537_a_406) {
    const auto expected = MillerRabinResult::IsLikelyPrime;
    const auto actual = miller_rabin_round(65537, 406);
    EXPECT_EQ(expected, actual);
}

// Examples from Hoffstein, Pipher, Silverman. An Introduction to Mathematical Cryptography, p.132f
// 561 is a Carmichael number, but with a = 2 we expect to recognize it as composite
TEST(MRTTests, Round_n_561_a_2) {
    const auto expected = MillerRabinResult::IsComposite;
    const auto actual = miller_rabin_round(561, 2);
    EXPECT_EQ(expected, actual);
}

// 172947529 is a Carmichael number and a = 17 is no Miller-Rabin witness
TEST(MRTTests, Round_n_172947529_a_17) {
    const auto expected = MillerRabinResult::IsLikelyPrime;
    const auto actual = miller_rabin_round(172947529, 17);
    EXPECT_EQ(expected, actual);
}

// 172947529 is a Carmichael number and a = 3 is no Miller-Rabin witness
TEST(MRTTests, Round_n_172947529_a_3) {
    const auto expected = MillerRabinResult::IsLikelyPrime;
    const auto actual = miller_rabin_round(172947529, 3);
    EXPECT_EQ(expected, actual);
}

// 172947529 is a Carmichael number but a = 23 is a Miller-Rabin witness
TEST(MRTTests, Round_n_172947529_a_23) {
    const auto expected = MillerRabinResult::IsComposite;
    const auto actual = miller_rabin_round(172947529, 23);
    EXPECT_EQ(expected, actual);
}




// --------------- Miller-Rabin-Test Tests -------------------
// Having n == 0 should throw an exception
TEST(MRTTests, Test_n_0) {
    EXPECT_THROW({
        try {
            miller_rabin_test(0);
        } catch(const std::invalid_argument& e) {
            // This tests that the exception has the correct message
            EXPECT_STREQ("0 is neither prime nor composite!", e.what());
            throw;
        }
    }, std::invalid_argument);
}

// Having n == 1 should throw an exception
TEST(MRTTests, Test_n_1) {
    EXPECT_THROW({
        try {
            miller_rabin_test(1);
        } catch(const std::invalid_argument& e) {
            // This tests that the exception has the correct message
            EXPECT_STREQ("1 is neither prime nor composite!", e.what());
            throw;
        }
    }, std::invalid_argument);
}

// Having num_rounds == 0 should throw an exception
TEST(MRTTests, Test_num_rounds_0) {
    EXPECT_THROW({
        try {
            miller_rabin_test(42, 0);
        } catch(const std::invalid_argument& e) {
            // This tests that the exception has the correct message
            EXPECT_STREQ("Cannot decide with no round. Please use at least 10 rounds!", e.what());
            throw;
        }
    }, std::invalid_argument);
}

// 65537 is prime, so we expect to not find any witnesses, even with many rounds
TEST(MRTTests, Test_n_65537_num_rounds_100) {
    const auto expected = MillerRabinResult::IsLikelyPrime;
    const auto actual = miller_rabin_test(65537, 255);
    EXPECT_EQ(expected, actual);
}

TEST(MRTTests, Test_n_2) {
    EXPECT_EQ(MillerRabinResult::IsLikelyPrime, miller_rabin_test(2));
}

TEST(MRTTests, Test_n_3) {
    EXPECT_EQ(MillerRabinResult::IsLikelyPrime, miller_rabin_test(3));
}

TEST(MRTTests, Test_large_prime) {
    EXPECT_EQ(MillerRabinResult::IsLikelyPrime, miller_rabin_test(4'294'967'291));
}

// 20 is composite, so we expect to find a witness
TEST(MRTTests, Test_n_20) {
    const auto expected = MillerRabinResult::IsComposite;
    const auto actual = miller_rabin_test(20);
    EXPECT_EQ(expected, actual);
}

// Examples from Hoffstein, Pipher, Silverman. An Introduction to Mathematical Cryptography, p.132f
// 561 is a Carmichael number, but with a = 2 we expect to recognize it as composite
TEST(MRTTests, Test_n_561) {
    const auto expected = MillerRabinResult::IsComposite;
    const auto actual = miller_rabin_test(561);
    EXPECT_EQ(expected, actual);
}

// 172947529 is a Carmichael number and a = 17 is no Miller-Rabin witness
TEST(MRTTests, Test_n_172947529) {
    const auto expected = MillerRabinResult::IsComposite;
    const auto actual = miller_rabin_test(172947529);
    EXPECT_EQ(expected, actual);
}