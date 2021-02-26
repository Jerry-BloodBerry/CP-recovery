#include "Charset.h"
#include <iostream>

Charset::Charset(int* charset, unsigned int charset_count)
{
    this->charset = charset;
    this->charset_count = charset_count;
}

Charset::Charset(std::string symbols)
{
    this->charset = charset_from_symbols(symbols);
    this->charset_count = get_size_for_symbols(symbols);
}

Charset::Charset()
{
    this->charset = charset_from_symbols("Aa0%");
    this->charset_count = get_size_for_symbols("Aa0%");
}

int Charset::get_size_for_symbols(std::string symbols) {
    int size = 0;
    int symbols_length = symbols.length();
    for (int i = 0; i < symbols_length; i++)
    {
        if (symbols[i] == 'a' || symbols[i] == 'A') {
            size += 26;
        }
        else if (symbols[i] == '0') {
            size += 10;
        }
        else if (symbols[i] == '%') {
            size += 33;
        }
        else {
            throw std::runtime_error("Undefined symbol in function get_array_size_for_symbols");
        }
    }
    return size;
}

int* Charset::charset_from_symbols(std::string symbols)
{
    bool containsSmallLetters = symbols.find('a') != std::string::npos;
    bool containsBigLetters = symbols.find('A') != std::string::npos;
    bool containsNumbers = symbols.find('0') != std::string::npos;
    bool containsSymbols = symbols.find('%') != std::string::npos;
    unsigned int charset_size = get_size_for_symbols(symbols);
    int* charset = new int[charset_size];
    int currentIndex = 0;
    if (containsSmallLetters) {
        appendSmallLetters(charset, currentIndex);
        currentIndex += 26;
    }
    if (containsBigLetters) {
        appendBigLetters(charset, currentIndex);
        currentIndex += 26;
    }
    if (containsNumbers) {
        appendNumbers(charset, currentIndex);
        currentIndex += 10;
    }
    if (containsSymbols) {
        appendSpecialCharacters(charset, currentIndex);
        currentIndex += 33;
    }
    return charset;
}

void Charset::appendSmallLetters(int* &charset, int currentIndex)
{
    for (int i = 97; i < 123; ++i) {
        charset[currentIndex] = i;
        ++currentIndex;
    }

}

void Charset::appendBigLetters(int* &charset, int currentIndex)
{
    for (int i = 65; i < 91; ++i) {
        charset[currentIndex] = i;
        ++currentIndex;
    }
}

void Charset::appendNumbers(int* &charset, int currentIndex)
{
    for (int i = 48; i < 58; ++i) {
        charset[currentIndex] = i;
        ++currentIndex;
    }
}

void Charset::appendSpecialCharacters(int* &charset, int currentIndex)
{
    for (int i = 32; i < 48; ++i) {
        charset[currentIndex] = i;
        ++currentIndex;
    }
    for (int i = 58; i < 65; ++i) {
        charset[currentIndex] = i;
        ++currentIndex;
    }
    for (int i = 91; i < 97; ++i) {
        charset[currentIndex] = i;
        ++currentIndex;
    }
    for (int i = 123; i < 127; ++i) {
        charset[currentIndex] = i;
        ++currentIndex;
    }
}

Charset::~Charset()
{
}