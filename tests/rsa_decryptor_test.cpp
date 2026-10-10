#include <gtest/gtest.h>
#include <numeric>

#include "rsa_decryptor.hpp"
#include "square_and_multiply.hpp"
#include "rsa_encryptor.hpp"

// Use Test Fixtures to test the RSADecryptor class
class RSADecryptorTest : public ::testing::Test {
    protected:
        RSADecryptor m_decryptor{};
        RSAEncryptor m_encryptor{};

#ifdef PRIVATE_TESTS
        auto get_sk() const  { return m_decryptor.m_sk; }
        auto get_phi() const { return m_decryptor.calc_phi(); }
        auto decrypt_cipher(const std::uint64_t c) const { return m_decryptor.decrypt_cipher(c); }
#endif
};

// Tests whether n > 0 and e > 1
TEST_F(RSADecryptorTest, PublicKeyIsValid) {
    const auto& pk = m_decryptor.get_public_key();
    EXPECT_GT(pk.n, 0ULL);
    EXPECT_GT(pk.e, 1ULL);
}

TEST(RSADecryptorConstructionTest, CreatesKeyFromSuppliedPrimes) {
    constexpr std::uint32_t p = 1009;
    constexpr std::uint32_t q = 1013;
    RSADecryptor decryptor{p, q};
    const PublicKey public_key = decryptor.get_public_key();

    const std::uint64_t phi =
        static_cast<std::uint64_t>(p - 1) * static_cast<std::uint64_t>(q - 1);
    EXPECT_EQ(public_key.n, static_cast<std::uint64_t>(p) * q);
    EXPECT_EQ(std::gcd(public_key.e, phi), 1ULL);
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

#ifdef PRIVATE_TESTS
// Test whether private key fulfills $e * d \equiv 1 \pmod(\phi(n))$
TEST_F(RSADecryptorTest, CheckPrivateKeyCorrectness) {
    const auto expected = 1;
    const auto actual = mul_mod(m_decryptor.get_public_key().e, get_sk(), get_phi());
    EXPECT_EQ(expected, actual);
}

// Encrypt and decrypt a test message to see if they match
TEST_F(RSADecryptorTest, EncryptDecryptRoundtrip1) {
    const auto pk = m_decryptor.get_public_key();
    const std::uint64_t msg = 2;
    const auto encrypted = square_and_multiply(msg, pk.e, pk.n);
    const auto decrypted = decrypt_cipher(encrypted);
    EXPECT_EQ(msg, decrypted);
}

// Encrypt and decrypt a test message to see if they match
TEST_F(RSADecryptorTest, EncryptDecryptRoundtrip2) {
    const auto pk = m_decryptor.get_public_key();
    const std::uint64_t msg = 180'502'720'646'772'809;
    const auto encrypted = square_and_multiply(msg, pk.e, pk.n);
    const auto decrypted = decrypt_cipher(encrypted);
    EXPECT_EQ(msg, decrypted);
}
#endif

// Do a roundtrip with the encryptor :)
TEST_F(RSADecryptorTest, EncryptDecryptRoundtrip3) {
    const std::string expected = "Hello from RSA encryptor!";
    const RSADecryptor decryptor{1'506'792'299, 1'829'390'351, 65537};
    const RSAEncryptor encryptor{decryptor.get_public_key()};
    const auto encrypted = encryptor.encrypt_msg(expected);
    const auto actual = decryptor.decrypt_msg(encrypted);
    EXPECT_EQ(expected, actual);
}