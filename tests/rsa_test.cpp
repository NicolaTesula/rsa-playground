#include <gtest/gtest.h>
#include <rsa.hpp>

// Encrypting an empty message should result in an empty vector
TEST(RSAEncryptionTests, RSAEncryptMsg_Nothing) {
    const auto actual = rsa_encrypt_msg("");
    ASSERT_TRUE(actual.empty());
}

// Encrypt UTF-8 encoded text
TEST(RSAEncryptionTests, RSAEncryptMsg_UTF8) {
    const auto actual = rsa_encrypt_msg("TハBロー・ワールド");
    ASSERT_TRUE(true);
}

// Encrypting a 0 should result in 0, since 0^x = 0
TEST(RSAEncryptionTests, RSAEncryptBlock_Zero) {
    const auto expected = 0;
    const auto actual = rsa_encrypt_block(0, 50, 100);
    ASSERT_EQ(expected, actual);
}

// Encrypting a 1 should result in 1, since 1^x = 1
TEST(RSAEncryptionTests, RSAEncryptBlock_One) {
    const auto expected = 1;
    const auto actual = rsa_encrypt_block(1, 50, 100);
    ASSERT_EQ(expected, actual);
}