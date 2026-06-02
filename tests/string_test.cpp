#include <gtest/gtest.h>

extern "C" {
#include "../monitor/string.h"
}

TEST(string_pad, pad) {
    pad_init();
    EXPECT_EQ(pad_len(), 0);
    pad_hold('a');
    EXPECT_EQ(pad_len(), 1);
    pad_hold('b');
    EXPECT_EQ(pad_len(), 2);

    // TODO: Check assert occurs when too much in pad
}

TEST(string_number, zero) {
    EXPECT_STREQ(utoa_pad(0, 10), "0");
    EXPECT_STREQ(itoa_pad(0, 10), "0");
    EXPECT_STREQ(utoa_pad(0, 2), "0");
    EXPECT_STREQ(itoa_pad(0, 2), "0");
    EXPECT_STREQ(utoa_pad(0, 16), "0");
    EXPECT_STREQ(itoa_pad(0, 16), "0");
}

TEST(string_number, zero_width) {
    EXPECT_STREQ(utoa_pad_w(0, 10, 0, false), "0");
    EXPECT_STREQ(itoa_pad_w(0, 10, 0, false), "0");
    EXPECT_STREQ(utoa_pad_w(0, 2,  0, false), "0");
    EXPECT_STREQ(itoa_pad_w(0, 2,  0, false), "0");
    EXPECT_STREQ(utoa_pad_w(0, 16, 0, false), "0");
    EXPECT_STREQ(itoa_pad_w(0, 16, 0, false), "0");

    EXPECT_STREQ(utoa_pad_w(0, 10, 1, false), "0");
    EXPECT_STREQ(itoa_pad_w(0, 10, 1, false), "0");
    EXPECT_STREQ(utoa_pad_w(0, 2,  1, false), "0");
    EXPECT_STREQ(itoa_pad_w(0, 2,  1, false), "0");
    EXPECT_STREQ(utoa_pad_w(0, 16, 1, false), "0");
    EXPECT_STREQ(itoa_pad_w(0, 16, 1, false), "0");

    EXPECT_STREQ(utoa_pad_w(0, 10, 2, false), " 0");
    EXPECT_STREQ(itoa_pad_w(0, 10, 2, false), " 0");
    EXPECT_STREQ(utoa_pad_w(0, 2,  2, false), " 0");
    EXPECT_STREQ(itoa_pad_w(0, 2,  2, false), " 0");
    EXPECT_STREQ(utoa_pad_w(0, 16, 2, false), " 0");
    EXPECT_STREQ(itoa_pad_w(0, 16, 2, false), " 0");

    EXPECT_STREQ(utoa_pad_w(0, 10, 2, true), "00");
    EXPECT_STREQ(itoa_pad_w(0, 10, 2, true), "00");
    EXPECT_STREQ(utoa_pad_w(0, 2,  2, true), "00");
    EXPECT_STREQ(itoa_pad_w(0, 2,  2, true), "00");
    EXPECT_STREQ(utoa_pad_w(0, 16, 2, true), "00");
    EXPECT_STREQ(itoa_pad_w(0, 16, 2, true), "00");
}

TEST(string_number, decimal_unsigned) {
    EXPECT_STREQ(utoa_pad(123, 10), "123");
    EXPECT_STREQ(utoa_pad(5, 10), "5");
}

TEST(string_number, decimal_unsigned_width) {
    EXPECT_STREQ(utoa_pad_w(123, 10, 0, false),  "123");
    EXPECT_STREQ(utoa_pad_w(5, 10, 0, false),      "5");
    EXPECT_STREQ(utoa_pad_w(123, 10, 4, false), " 123");
    EXPECT_STREQ(utoa_pad_w(5, 10, 4, false),   "   5");

    EXPECT_STREQ(utoa_pad_w(123, 10, 0, true),  "123");
    EXPECT_STREQ(utoa_pad_w(5, 10, 0, true),      "5");
    EXPECT_STREQ(utoa_pad_w(123, 10, 4, true), "0123");
    EXPECT_STREQ(utoa_pad_w(5, 10, 4, true),   "0005");
}

TEST(string_number, decimal_signed) {
    EXPECT_STREQ(itoa_pad(123, 10),   "123");
    EXPECT_STREQ(itoa_pad(-123, 10), "-123");

    EXPECT_STREQ(itoa_pad(5, 10),   "5");
    EXPECT_STREQ(itoa_pad(-5, 10), "-5");
}

TEST(string_number, decimal_signed_width) {
    EXPECT_STREQ(itoa_pad_w(123, 10, 0, false),    "123");
    EXPECT_STREQ(itoa_pad_w(-123, 10, 0, false),  "-123");
    EXPECT_STREQ(itoa_pad_w(123, 10, 4, false),   " 123");
    EXPECT_STREQ(itoa_pad_w(-123, 10, 4, false),  "-123");

    EXPECT_STREQ(itoa_pad_w(5, 10, 0, false),     "5");
    EXPECT_STREQ(itoa_pad_w(-5, 10, 0, false),   "-5");
    EXPECT_STREQ(itoa_pad_w(5, 10, 4, false),  "   5");
    EXPECT_STREQ(itoa_pad_w(-5, 10, 4, false), "-  5");

    EXPECT_STREQ(itoa_pad_w(123, 10, 0, true),    "123");
    EXPECT_STREQ(itoa_pad_w(-123, 10, 0, true),  "-123");
    EXPECT_STREQ(itoa_pad_w(123, 10, 4, true),   "0123");
    EXPECT_STREQ(itoa_pad_w(-123, 10, 4, true),  "-123");

    EXPECT_STREQ(itoa_pad_w(5, 10, 0, true),     "5");
    EXPECT_STREQ(itoa_pad_w(-5, 10, 0, true),   "-5");
    EXPECT_STREQ(itoa_pad_w(5, 10, 4, true),  "0005");
    EXPECT_STREQ(itoa_pad_w(-5, 10, 4, true), "-005");
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

TEST(string_number, hex_sep) {
    EXPECT_STREQ(itoa_pad(0x1234BEEF, 16),   "1234_BEEF");
    EXPECT_STREQ(itoa_pad(-0x1234BEEF, 16), "-1234_BEEF");
    EXPECT_STREQ(itoa_pad(0xDBEEF, 16),         "D_BEEF");
    EXPECT_STREQ(itoa_pad(-0xDBEEF, 16),       "-D_BEEF");
}

TEST(string_number, hex_sep_width) {
    EXPECT_STREQ(itoa_pad_w(0, 16, 4, true),       "0000");
    EXPECT_STREQ(itoa_pad_w(0, 16, 5, true),     "0_0000");
    EXPECT_STREQ(itoa_pad_w(0, 16, 8, true),  "0000_0000");
    EXPECT_STREQ(itoa_pad_w(0, 16, 4, false),      "   0");
    EXPECT_STREQ(itoa_pad_w(0, 16, 5, false),     "    0");
}

TEST(number_string, strtou) {
    unsigned u;
    char *endptr;

    EXPECT_EQ(strtou("123", NULL, 10), 123);

    EXPECT_EQ(strtou("456", &endptr, 10), 456);
    EXPECT_EQ(*endptr, '\0');

    EXPECT_EQ(strtou("abc", &endptr, 10), 0);
    EXPECT_EQ(*endptr, 'a');

    EXPECT_EQ(strtou("12cd", &endptr, 10), 12);
    EXPECT_EQ(*endptr, 'c');

    EXPECT_EQ(strtou("deadz", &endptr, 16), 0xDEAD);
    EXPECT_EQ(*endptr, 'z');
}

TEST(number_string, strtoi) {
    EXPECT_EQ(strtoi("123", NULL, 10), 123);
    EXPECT_EQ(strtoi("-123", NULL, 10), -123);
}
