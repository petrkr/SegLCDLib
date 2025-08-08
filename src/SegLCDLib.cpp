#include <Wire.h>
#include <SegLCDLib.h>


void SegLCDLib::clear() {
    home();
}

void SegLCDLib::home() {
    setCursor(0, 0);
}

void SegLCDLib::setCursor(uint8_t row, uint8_t col) {
    _cursorRow = row;
    _cursorCol = col;
}

//Mapping ABCD EFGH
uint8_t SegLCDLib::_get_char_value(char ch) {
    switch (ch) {
        case ' ': return 0x00;
        case '0': return 0b11111100;
        case '1': return 0b01100000;
        case '2': return 0b11011010;
        case '3': return 0b11110010;
        case '4': return 0b01100110;
        case '5': return 0b10110110;
        case '6': return 0b10111110;
        case '7': return 0b11100000;
        case '8': return 0b11111110;
        case '9': return 0b11110110;
        case 'A':
        case 'a': return 0b11101110;
        case 'B':
        case 'b': return 0b00111110;
        case 'C': return 0b10011100;
        case 'c': return 0b00011010;
        case 'D':
        case 'd': return 0b01111010;
        case 'E':
        case 'e': return 0b10011110;
        case 'F':
        case 'f': return 0b10001110;
        case 'G':
        case 'g': return 0b11110110;
        case 'H': return 0b01101110;
        case 'h': return 0b00101110;
        case 'I': return 0b01100000;
        case 'i': return 0b00100000;
        case 'J': return 0b01111000;
        case 'j': return 0b01111000;
        case 'K':
        case 'k': return 0b10101110;
        case 'L': return 0b00011100;
        case 'l': return 0b00011000;
        case 'M':
        case 'm': return 0b10101010;
        case 'N': return 0b11101100;
        case 'n': return 0b00101010;
        case 'O': return 0b11111100;
        case 'o': return 0b00111010;
        case 'P':
        case 'p': return 0b11001110;
        case 'Q':
        case 'q': return 0b11100110;
        case 'R':
        case 'r': return 0b00001010;
        case 'S':
        case 's': return 0b10110110;
        case 'T':
        case 't': return 0b00011110;
        case 'U': return 0b01111100;
        case 'u': return 0b00111000;
        case 'V':
        case 'v': return 0b01010100;
        case 'W':
        case 'w': return 0b01010110;
        case 'X': return 0b01101110;
        case 'x': return 0b00101000;
        case 'Y':
        case 'y': return 0b01110110;
        case 'Z': return 0b11011010;
        case 'z': return 0b11011000;

        case '*': return 0b11000110;
        case ',': return 0b00001000;
        case '.': return 0b00010000;
        case '/': return 0b01001010;
        case '~': return 0b10000000;
        case '-': return 0b00000010;
        case '_': return 0b00010000;
        case ':': return 0b00010010;
        case '|': return 0b00001100;
        case '"': return 0b01000100;
        case '(':
        case '[': return 0b10011100;
        case ')':
        case ']': return 0b11110000;

        default: return 0x00;
    }
}

//Mapping A1A2BC D2D1EF G1G2HI JKLM
uint16_t SegLCDLib::_get_16char_value(char ch) {
    switch (ch) {
        case ' ': return 0b0000000000000000;
        case '0': return 0b1111111100001100;
        case '1': return 0b0011000000000000;
        case '2': return 0b1110111011000000;
        case '3': return 0b1111110011000000;
        case '4': return 0b0011000111000000;
        case '5': return 0b1101110111000000;
        case '6': return 0b1101111111000000;
        case '7': return 0b1111000000000000;
        case '8': return 0b1111111111000000;
        case '9': return 0b1111110111000000;

        case 'A': return 0b1111001111000000;
        case 'a': return 0b0000111010000010;
        case 'B': return 0b1111110001010010;
        case 'b': return 0b0001111111000000;
        case 'C': return 0b1100111100000000;
        case 'c': return 0b0000111011000000;
        case 'D': return 0b1111110000010010;
        case 'd': return 0b0011111011000000;
        case 'E': return 0b1100111110000000;
        case 'e': return 0b0000111010000100;
        case 'F': return 0b1100001110000000;
        case 'f': return 0b0100000011010010;
        case 'G': return 0b1101111101000000;
        case 'g': return 0b0000101010000101;
        case 'H': return 0b0011001111000000;
        case 'h': return 0b0001001111000000;
        case 'I': return 0b1100110000010010;
        case 'i': return 0b1000110010000010;
        case 'J': return 0b0011111000000000;
        case 'j': return 0b0101110001000000;
        case 'K': return 0b0000001110001001;
        case 'k': return 0b0000001111000001;
        case 'L': return 0b0000111100000000;
        case 'l': return 0b1000100000010010;
        case 'M': return 0b0011001100101000;
        case 'm': return 0b0001001011000010;
        case 'N': return 0b0011001100100001;
        case 'n': return 0b0001001011000000;
        case 'O': return 0b1111111100000000;
        case 'o': return 0b0001111011000000;
        case 'P': return 0b1110001111000000;
        case 'p': return 0b0001010001000011;
        case 'Q': return 0b1111111100000001;
        case 'q': return 0b0001111011000001;
        case 'R': return 0b1110001111000001;
        case 'r': return 0b0000001011000000;
        case 'S': return 0b1101110111000000;
        case 's': return 0b0000110001000001;
        case 'T': return 0b1100000000010010;
        case 't': return 0b0000100011010010;
        case 'U': return 0b0011111100000000;
        case 'u': return 0b0001111000000000;
        case 'V': return 0b0000001100001100;
        case 'v': return 0b0000001000000100;
        case 'W': return 0b0011001100000101;
        case 'w': return 0b0001001000000101;
        case 'X': return 0b0000000000101101;
        case 'x': return 0b0000000011000101;
        case 'Y': return 0b0000000000101010;
        case 'y': return 0b0001110000000001;
        case 'Z': return 0b1100110000001100;
        case 'z': return 0b0000010010000100;

        case '*': return 0b0000000011111111;
        case ',': return 0b0000000000000100;
        case '.': return 0b0000010000000000;
        case '/': return 0b0000000000001100;
        case '\\': return 0b0000000000100001;
        case '~': return 0b1100000000000000;
        case '+': return 0b0000000011010010;
        case '-': return 0b0000000011000000;
        case '_': return 0b0000110000000000;
        case ':': return 0b0000010010000000;
        case '=': return 0b0000110011000000;
        case '|': return 0b0000000000010010;
        case '"': return 0b0000000000101000;
        case '(': return 0b0000000000001001;
        case ')': return 0b0000000000100100;
        case '[': return 0b0100100000010010;
        case ']': return 0b1000010000010010;
        case '{': return 0b0100100010010010;
        case '}': return 0b1000010001010010;

        default: return 0b0000000000000000;
    }
}

