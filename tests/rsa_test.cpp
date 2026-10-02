#include <gtest/gtest.h>
#include <rsa.hpp>

// Encrypting an empty message should result in... TODO
TEST(RSAEncryptTests, RSAEncrypt_Nothing) {
    const auto expected = 0;
    const auto actual = rsa_encrypt("");
    ASSERT_EQ(expected, actual);
}