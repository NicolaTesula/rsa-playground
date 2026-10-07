#include <gtest/gtest.h>

#include "rsa_encryptor.hpp"
#include "square_and_multiply.hpp"

class RSAEncryptorTest : public ::testing::Test {
    protected:
        RSAEncryptor m_encryptor{};

#ifdef PRIVATE_TESTS
#endif
};


// Test the encryption of "Hello World!"
TEST_F(RSAEncryptorTest, Encrypt_HelloWorld) {
    PublicKey pk = {2'756'511'292'751'706'949ULL, 65537};
    m_encryptor.set_public_key(pk);
    const std::uint64_t block1 = square_and_multiply(0x00'48'65'6C'6C'6F'20'57ULL, pk.e, pk.n);
    const std::uint64_t block2 = square_and_multiply(0x00'6F'72'6C'64'21'00'00ULL, pk.e, pk.n);
    const auto expected = std::vector<std::uint64_t>{block1, block2};
    const auto actual = m_encryptor.encrypt_msg("Hello World!");
    EXPECT_EQ(expected, actual);
}