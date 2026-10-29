#include <gtest/gtest.h>

#include "fermat_factorization.hpp"

// Passing a number smaller than 4 should throw
TEST(FermatTests, Test_num_rounds_0) {
    EXPECT_THROW({
        try {
            fermat_factorization(3);
        } catch(const std::invalid_argument& e) {
            // This tests that the exception has the correct message
            EXPECT_STREQ("n must be a composite integer greater than 1!", e.what());
            throw;
        }
    }, std::invalid_argument);
}

// 77 = 7 * 11 should be correctly identified
TEST(FermatTests, n_77) {
    const PrimeFactors factors = fermat_factorization(77);

    const std::set<std::uint64_t> expected{7, 11};
    const std::set<std::uint64_t> actual{factors.p, factors.q};
    EXPECT_EQ(expected, actual);
}

// Examples from Hoffstein, Pipher, Silverman. An Introduction to Mathematical Cryptography, p.139 f.
// 25217 = 167 * 151 should be correctly identified
TEST(FermatTests, n_25217) {
    const PrimeFactors factors = fermat_factorization(25217);

    const std::set<std::uint64_t> expected{167, 151};
    const std::set<std::uint64_t> actual{factors.p, factors.q};
    EXPECT_EQ(expected, actual);
}

// 203299 = 263 * 773
TEST(FermatTests, n_203299) {
    const PrimeFactors factors = fermat_factorization(203299);

    const std::set<std::uint64_t> expected{263, 773};
    const std::set<std::uint64_t> actual{factors.p, factors.q};
    EXPECT_EQ(expected, actual);
}