#include <gtest/gtest.h>

extern "C" {
#include "../monitor/char.h"
}

TEST(char_type, isprint) {
    for (int i = 0; i <= 0xFF; i++) {
        char c = static_cast<char>(i);
        EXPECT_EQ(char_isprint(c), !!std::isprint(c));
    }
}

TEST(case_conversion, uppercase) {
    for (char c = 'a'; c <= 'z'; c++) {
        EXPECT_EQ(char_to_upper(c), std::toupper(c));
    }
}

TEST(digit_conversion, digits) {
    const char *digit_chars{"0123456789ABCDEF"};
    for (int i = 0; i <= 0x0F; i++) {
        EXPECT_EQ(digit_to_char(i), digit_chars[i]);
        EXPECT_EQ(char_to_digit(digit_to_char(i)), i);
    }
}

TEST(digit_conversion, lowercase) {
    for (char c = 'a'; c <= 'f'; c++) {
        EXPECT_EQ(digit_to_char(char_to_digit(c)), std::toupper(c));
    }
}

TEST(digit_conversion, notfound) {
    EXPECT_EQ(digit_to_char(0xFF), '\0');
    EXPECT_EQ(char_to_digit('Z'), 0xFF);
}
