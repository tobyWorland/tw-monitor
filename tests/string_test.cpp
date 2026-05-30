#include <gtest/gtest.h>

extern "C" {
#include "../monitor_src/string.h"
}

TEST(string_number, zero) {
    EXPECT_STREQ(utoa_pad(0, 10), "0");
    EXPECT_STREQ(itoa_pad(0, 10), "0");
    EXPECT_STREQ(utoa_pad(0, 2), "0");
    EXPECT_STREQ(itoa_pad(0, 2), "0");
    EXPECT_STREQ(utoa_pad(0, 16), "0");
    EXPECT_STREQ(itoa_pad(0, 16), "0");
}

TEST(string_number, decimal_unsigned) {
    EXPECT_STREQ(utoa_pad(123, 10), "123");
    EXPECT_STREQ(utoa_pad(5, 10), "5");
}

TEST(string_number, decimal_signed) {
    EXPECT_STREQ(itoa_pad(123, 10), "123");
    EXPECT_STREQ(itoa_pad(-123, 10), "-123");

    EXPECT_STREQ(itoa_pad(5, 10), "5");
    EXPECT_STREQ(itoa_pad(-5, 10), "-5");
}

TEST(string_number, binary_unsigned) {
    EXPECT_STREQ(utoa_pad(5, 2), "101");
    EXPECT_STREQ(utoa_pad(15, 2), "1111");
}

TEST(string_number, binary_signed) {
    EXPECT_STREQ(itoa_pad(5, 2), "101");
    EXPECT_STREQ(itoa_pad(-5, 2), "-101");

    EXPECT_STREQ(itoa_pad(15, 2), "1111");
    EXPECT_STREQ(itoa_pad(-15, 2), "-1111");
}

TEST(string_number, hex_unsigned) {
    EXPECT_STREQ(itoa_pad(5, 16), "5");
    EXPECT_STREQ(itoa_pad(16, 16), "10");
    EXPECT_STREQ(itoa_pad(0xCAFE, 16), "CAFE");
}

TEST(string_number, hex_signed) {
    EXPECT_STREQ(itoa_pad(5, 16), "5");
    EXPECT_STREQ(itoa_pad(16, 16), "10");
    EXPECT_STREQ(itoa_pad(0xCAFE, 16), "CAFE");

    EXPECT_STREQ(itoa_pad(-5, 16), "-5");
    EXPECT_STREQ(itoa_pad(-16, 16), "-10");
    EXPECT_STREQ(itoa_pad(-0xCAFE, 16), "-CAFE");
}
