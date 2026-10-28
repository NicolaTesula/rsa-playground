#include <gtest/gtest.h>

#ifdef DEBUG
#include <iostream>
#endif

#include "block_encoding.hpp"

// Too large block width should result in exception
TEST(BlockEncodingTest, TooLargeBlockWidth) {
    EXPECT_THROW({
        try {
            string_to_blocks("Hello World!", 65);
        } catch(const std::invalid_argument& e) {
            EXPECT_STREQ("One block cannot be wider than 64 bit!", e.what());
            throw;
        }
    }, std::invalid_argument);
}

// "Hello World!" is 12 bytes of ASCII, so expect 0x48 65 6C 6C 6F 20 57 6F 72 6C 64 21 00 00 00 00
TEST(BlockEncodingTest, StrToBlock_HelloWorld1) {
    const std::vector expected = {0x48'65'6C'6C'6F'20'57'6FULL, 0x72'6C'64'21'00'00'00'00ULL};
    const auto actual = string_to_blocks("Hello World!", 64);
    EXPECT_EQ(expected, actual);
}

// "Hello World!" is 12 bytes of ASCII. With 56 Bits per block we expect 0x00 48 65 6C 6C 6F 20 57 | 00 6F 72 6C 64 21 00 00 00
TEST(BlockEncodingTest, StrToBlock_HelloWorld2) {
    const std::vector expected = {0x00'48'65'6C'6C'6F'20'57ULL, 0x00'6F'72'6C'64'21'00'00ULL};
    const auto actual = string_to_blocks("Hello World!", 56);
    EXPECT_EQ(expected, actual);
}

// TODO: Test UTF-8 encoded string like こんにちは、世界

// Correctly encoded "Hello World!" should return "Hello World!" on decoding again
TEST(BlockEncodingTest, BlockToStr_HelloWorldVec1) {
    const std::vector<std::uint64_t> str_vec = {0x48'65'6C'6C'6F'20'57'6FULL, 0x72'6C'64'21'00'00'00'00ULL};
    const std::string expected = "Hello World!";
    const auto actual = blocks_to_string(str_vec, 64);
#ifdef DEBUG 
    std::cout << "Expected: " << expected << " Actual: " << actual << '\n';
#endif
    EXPECT_EQ(expected, actual);
}