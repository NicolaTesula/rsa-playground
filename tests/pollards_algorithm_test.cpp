#include <gtest/gtest.h>

#include <set>

#include "pollards_algorithm.hpp"

// Having modul 0 should always throw an exception
TEST(PollardTests, n_3) {
    EXPECT_THROW({
        try {
            pollards_algorithm(3);
        } catch(const std::invalid_argument& e) {
            // This tests that the exception has the correct message
            EXPECT_STREQ("n must be a composite integer greater than 1!", e.what());
            throw;
        }
    }, std::invalid_argument);
}

// TODO: find test for n where pollard can't find p

// Should be able to factor n = 77 = 7 * 11
TEST(PollardTests, n_77) {
    const auto factors = pollards_algorithm(77);

    const std::set<std::uint64_t> expected{7, 11};
    const std::set<std::uint64_t> actual{factors.p, factors.q};

    EXPECT_EQ(expected, actual);
}

// Examples from Hoffstein, Pipher, Silverman. An Introduction to Mathematical Cryptography, p.139 f.
// Should be able to factor n = 13927189 = 3823 * 3643
TEST(PollardTests, n_13927189) {
    const auto factors = pollards_algorithm(13927189);

    const std::set<std::uint64_t> expected{3823, 3643};
    const std::set<std::uint64_t> actual{factors.p, factors.q};

    EXPECT_EQ(expected, actual);
}

// Should be able to factor n = 168441398857 = 350437 * 480661
TEST(PollardTests, n_168441398857) {
    const auto factors = pollards_algorithm(168441398857);

    const std::set<std::uint64_t> expected{350437, 480661};
    const std::set<std::uint64_t> actual{factors.p, factors.q};

    EXPECT_EQ(expected, actual);
}