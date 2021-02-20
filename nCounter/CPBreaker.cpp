#include "CPBreaker.h"
#include <string>

CPBreaker::CPBreaker(int* characters, int char_count, int pass_length)
{
    char_codes = characters;
    char_set_count = char_count;
    password_length = pass_length;
}

std::string CPBreaker::build_characters_string(std::string symbols)
{
    bool containsSmallLetters = symbols.find('a') != std::string::npos;
    bool containsBigLetters = symbols.find('A') != std::string::npos;
    bool containsNumbers = symbols.find('0') != std::string::npos;
    bool containsSymbols = symbols.find('%') != std::string::npos;

    std::string charactersString = "";
    if (containsSmallLetters) {
        for (int i = 97; i < 123; ++i) {
            charactersString += (char)i;
        }
    }
    if (containsBigLetters) {
        for (int i = 65; i < 91; ++i) {
            charactersString += (char)i;
        }
    }
    if (containsNumbers) {
        for (int i = 48; i < 58; ++i) {
            charactersString += (char)i;
        }
    }
    if (containsSymbols) {
        for (int i = 32; i < 48; ++i) {
            charactersString += (char)i;
        }
        for (int i = 58; i < 65; ++i) {
            charactersString += (char)i;
        }
        for (int i = 91; i < 97; ++i) {
            charactersString += (char)i;
        }
        for (int i = 123; i < 127; ++i) {
            charactersString += (char)i;
        }
    }
    return charactersString;
}

bool CPBreaker::increment_password_array(int* password_array)
{
    bool incrementationSucceeded = false;
    if (password_array[password_length - 1] + 1 != char_set_count) {
        ++password_array[password_length - 1];
        incrementationSucceeded = true;
    }
    else {
        int i = 0;
        while (i != password_length && password_array[password_length - 1 - i] + 1 == char_set_count) {
            password_array[password_length - 1 - i] = 0;
            ++i;
        }
        if (i != password_length) {
            ++password_array[password_length - 1 - i];
            incrementationSucceeded = true;
        }
    }
    return incrementationSucceeded;
}

std::string CPBreaker::build_password_string_from_array(int* passwordArray)
{
    std::string password;
    password.reserve(password_length);
    for (int i = 0; i < password_length; ++i) {
        password += (char)(char_codes[passwordArray[i]]);
    }
    return password;
}

CPBreaker::~CPBreaker()
{
    delete[] char_codes;
}