#include "keyboard.hpp"
#include <stdexcept>
#include <vector>
#include <map>
#include <string>

using namespace std;

// An input sequence may contain characters '0'-'9' (text characters), backspace
// character '*', break character ' ' and end character '#'.
// 
// A sequence of one or more of the same text character will translate to a
// single output character, as specified by KEYBOARD map below. A single text
// character will translate to the first character in the corresponding array,
// with each additional text character advancing to the next character along the
// array, wrapping around when it reaches the end.
// 
// Backspace character will erase the last output character.
// Todo: Find out if text characters input after a break character may modify an
// output character that just became the last one. For now I always start with a
// new output character.
//
// Break character signals the end of the current input sequence. This is needed
// to codify one or more consecutive output characters mapped by the same text
// character.
// Todo: Find out how the function should handle break character between two
// different text characters. Should I ignore/assume this will not happen or
// signal an error? For now I ignore.
//
// End character is always present at the end of the input string.

namespace {
// This is the text input keyboard. Consequent text input characters '0'-'9'
// will cycle through the characters to the right.
    const map<char, vector<char>> KEYBOARD = {
        { '0', { ' ' } },
        { '1', { '&', '\'', '(', ')' }}, // I took a liberty to add a closing brace to this button.
        { '2', { 'A', 'B', 'C' }},
        { '3', { 'D', 'E', 'F' }},
        { '4', { 'G', 'H', 'I' }},
        { '5', { 'J', 'K', 'L' }},
        { '6', { 'M', 'N', 'O' }},
        { '7', { 'P', 'Q', 'R', 'S' }},
        { '8', { 'T', 'U', 'V' }},
        { '9', { 'W', 'X', 'Y', 'Z' }},
    };

    const char STROKE_BACKSPACE = '*';
    const char STROKE_BREAK = ' ';
    const char STROKE_END = '#';

    const char OUTPUT_NONE = 0;
    const char STROKE_NONE = 0;
}

string old_phone_pad(const string & strokes) {
// Resulting text.
    string text;
// Last processed stroke. Is only valid when last non-finalized output character
// is not NONE.
    char stroke_last;
// Characters mapped by the last text character and index of the non-finalized
// output character among them. Are only valid when last non-finalized output
// character is not NONE.
    vector<char> outputs;
    int output_index;
// Non-finalized output character. May change if the next stroke is the same
// text character.
    char output = OUTPUT_NONE;

    for (const char & stroke : strokes) {
        switch (stroke) {
            case STROKE_BACKSPACE:
// If there's a non-finalized output character, clear it.
                if (output != OUTPUT_NONE) {
                    output = OUTPUT_NONE;
                } else
// Otherwise, if there are finalized output characters, clear the last one.
                if (text.length() > 0) {
                    text.pop_back();
                }
                break;

            case STROKE_BREAK:
// If there's a non-finalized output character, finalize it.
                if (output != OUTPUT_NONE) {
                    text.push_back(output);
                    output = OUTPUT_NONE;
                } else
// Todo: This could be invalid. Check with technical specification if this input
// sequence is possible, specifically a break character without a non-finalized
// output character and how it should be handled. For now I consider it an
// error.
                {
                    throw invalid_argument("A breaking character ' ' can only appear after a text input character '0'-'9'.");
                }
                break;

            case STROKE_END:
// If there's a non-finalized output character, finalize it.
                if (output != OUTPUT_NONE) {
                    text.push_back(output);
                }
// Quit.
                return text;

            default:
// If there's a non-finalized output character, check whether the next text
// character continues the output sequence or not.
                if (output != OUTPUT_NONE) {
// If the same text character is repeated, modify non-finalized output
// character.
                    if (stroke == stroke_last) {
                        if ((output_index += 1) == outputs.size()) {
                            output_index = 0;
                        }
                    } else
// Otherwise, finalize output character and continue to starting a new output
// character sequence.
                    {
                        text.push_back(output);
                        output = OUTPUT_NONE;
                    }
                }
// If there's no non-finalized output character, including the case when it has
// just been finalized, start a new output character sequence by detecting
// correct text character.
                if (output == OUTPUT_NONE) {
                    stroke_last = stroke;
                    try {
                        outputs = KEYBOARD.at(stroke);
                    }
                    catch (std::out_of_range) {
// This should never happen according to technical specification.
                        throw invalid_argument("Strokes sequence contains illegal character '" + string(1, stroke) + "'.");
                    }
                    output_index = 0;
                }
                output = outputs[output_index];
                break;
        }
    }

// This should never be reached according to technical specification.
    throw invalid_argument("Strokes sequence doesn't end with ending character '#'.");
}