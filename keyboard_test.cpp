#include "keyboard.hpp"
#include <vector>
#include <tuple>
#include <string>
#include <gtest/gtest.h>

using namespace std;

// Test input stroke sequences that translate to valid output texts.
TEST(keyboard_tests, keyboard_test_valid) {
    vector<tuple<string, string>> cases_valid = {
// Simple test cases with one output character. Including wrap arounds.
        { "0#", " " },
        { "00#", " " },
        { "1#", "&" },
        { "11#", "'" },
        { "111#", "(" },
        { "1111#", ")" },
        { "11111#", "&" },
        { "2#", "A" },
        { "22#", "B" },
        { "222#", "C" },
        { "2222#", "A" },
        { "3#", "D" },
        { "33#", "E" },
        { "333#", "F" },
        { "3333#", "D" },
        { "4#", "G" },
        { "44#", "H" },
        { "444#", "I" },
        { "4444#", "G" },
        { "5#", "J" },
        { "55#", "K" },
        { "555#", "L" },
        { "5555#", "J" },
        { "6#", "M" },
        { "66#", "N" },
        { "666#", "O" },
        { "6666#", "M" },
        { "7#", "P" },
        { "77#", "Q" },
        { "777#", "R" },
        { "7777#", "S" },
        { "77777#", "P" },
        { "8#", "T" },
        { "88#", "U" },
        { "888#", "V" },
        { "8888#", "T" },
        { "9#", "W" },
        { "99#", "X" },
        { "999#", "Y" },
        { "9999#", "Z" },
        { "99999#", "W" },
// Simple test cases for breaking character.
        { "0 0#", "  "},
        { "1 11 111 1111#", "&'()"},
// The following test cases for breaking character might be removed later if
// they are considered to be invalid or erroneous.
        { "0 1#", " &"},
        { "0 00 1#", "  &"},
// Simple test cases for backspace character.
        { "*0#", " " },
        { "0**1#", "&" },
        { "01*#", " " },
        { "011222**33334*#", " D" },
// A few simple strings.
        { "4433555 555666#", "HELLO" },
        { "8 88777444666*664#", "TURING" },
// A random string (with all options).
        { "207772222666663666 66660777787774444*444 4 *6640111944484402555 555**555 555066678444666 6677771111#", "A RANDOM STRING (WITH ALL OPTIONS)" }
    };

    for (tuple<string, string> case_valid : cases_valid) {
        const string & strokes = get<0>(case_valid);
        const string & text_expected = get<1>(case_valid);
        ASSERT_EQ(old_phone_pad(strokes), text_expected);
    }
}

// Test input stroke sequences that result in exceptions due to incorrect
// placement of breaking character ' '.
TEST(keyboard_tests, keyboard_test_error_breaking) {
// A simple case of a breaking character between two different text characters.
// For now it's not considered an error.
//    ASSERT_THROW(old_phone_pad("1 2#"), invalid_argument);

// A slightly more complicated case of a breaking character between two
// different text characters but with a valid breaking character before that.
// For now it's not considered an error.
//    ASSERT_THROW(old_phone_pad("1 11 2#"), invalid_argument);

// A case of a breaking character between two same text characters but with the
// preceding output character already finalized (thus, there's no need for a
// breaking character).
    ASSERT_THROW(old_phone_pad("1 11* 1#"), invalid_argument);

// A case of a breaking character between two different text characters and with
// with the preceding output character already finalized.
    ASSERT_THROW(old_phone_pad("1 11* 2#"), invalid_argument);

// A case of two consequent breaking characters.
    ASSERT_THROW(old_phone_pad("0  #"), invalid_argument);
}

int main(int argc, char ** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}