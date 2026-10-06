#include <gtest/gtest.h>

#include "rsa_decryptor.hpp"
#include "square_and_multiply.hpp"

// Use Test Fixtures to test the RSADecryptor class
class RSADecryptorTest : public ::testing::Test {
    protected:
        RSADecryptor decryptor{};

#ifdef PRIVATE_TESTS
        auto get_sk() const  { return decryptor.m_sk; }
        auto get_phi() const { return decryptor.calc_phi(); }
#endif
};

// Tests whether n > 0 and e > 1
TEST_F(RSADecryptorTest, PublicKeyIsValid) {
    const auto& pk = decryptor.get_public_key();
    EXPECT_GT(pk.n, 0ULL);
    EXPECT_GT(pk.e, 1ULL);
}

// Should throw an exception when trying to construct with too large bit width
TEST_F(RSADecryptorTest, ForbidLargeBitWidth) {
    EXPECT_THROW({
        try {
            RSADecryptor decryptor{32};
        } catch(const std::invalid_argument& e) {
            EXPECT_STREQ("31 bits is the max width supported right now!", e.what());
            throw;
        }
    }, std::invalid_argument);
}

// Encrypt and decrypt a test message to see if they match
TEST_F(RSADecryptorTest, EncryptDecryptRoundtrip1) {
    const auto& pk = decryptor.get_public_key();
    const std::uint64_t msg = 2;
    const auto encrypted = square_and_multiply(msg, pk.e, pk.n);
    const auto decrypted = decryptor.decrypt_cipher(encrypted);
    EXPECT_EQ(msg, decrypted);
}

// Encrypt and decrypt a test message to see if they match
TEST_F(RSADecryptorTest, EncryptDecryptRoundtrip2) {
    const auto& pk = decryptor.get_public_key();
    const std::uint64_t msg = 180'502'720'646'772'809;
    const auto encrypted = square_and_multiply(msg, pk.e, pk.n);
    const auto decrypted = decryptor.decrypt_cipher(encrypted);
    EXPECT_EQ(msg, decrypted);
}

// TODO: finish once encryptor is done
// TEST_F(RSADecryptorTest, EncryptDecryptRoundtrip3) {
//     const auto& pk = decryptor.get_public_key();
//     const std::uint64_t msg = 180'502'720'646'772'809;
//     const auto encrypted = square_and_multiply(msg, pk.e, pk.n);
//     const auto decrypted = decryptor.decrypt_cipher(encrypted);
//     EXPECT_EQ(msg, decrypted);
// }



// ------------ Private Tests ----------------
#ifdef PRIVATE_TESTS
// Test whether private key fulfills $e * d \equiv 1 \pmod(\phi(n))$
TEST_F(RSADecryptorTest, CheckPrivateKeyCorrectness) {
    const auto expected = 1;
    const auto actual = mul_mod(decryptor.get_public_key().e, get_sk(), get_phi());
    EXPECT_EQ(expected, actual);
}
#endif