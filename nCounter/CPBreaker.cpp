#include "CPBreaker.h"
#include <string>

CPBreaker::CPBreaker(int* characters, unsigned int char_count, unsigned int pass_length, std::string hash_string)
{
    char_codes = characters;
    char_set_count = char_count;
    password_length = pass_length;
    hash = hash_string;
}

int CPBreaker::get_array_size_for_symbols(std::string symbols) {
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

int* CPBreaker::build_characters_array(std::string symbols) {
    std::string characters_string = build_characters_string(symbols);
    int characters_count = characters_string.length();
    int* characters = new int[characters_count];
    for (int i = 0; i < characters_count; ++i) {
        characters[i] = characters_string[i];
    }
    return characters;
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

void CPBreaker::check_password_range(unsigned int startChar, unsigned int endChar)
{
    int* passwordArray = new int[password_length];
    passwordArray[0] = startChar;
    for (int i = 1; i < password_length; ++i) {
        passwordArray[i] = 0;
    }
    bool incrementationSucceeded = true;
    std::string password = "";
    while (incrementationSucceeded && !password_found) {
        password = build_password_string_from_array(passwordArray);
        bool result = Botan::argon2_check_pwhash(password.c_str(), password_length, "$argon2i$v=19$m=16,t=2,p=1$U000T0hQSjBQaUZCb0tBaw$o7WrL9n3BIV3AP5Q2Ax6sA");
        if (result) {
            password_found = true;
        }
        incrementationSucceeded = increment_password_array(passwordArray) && (passwordArray[0] != (char)endChar);
    }
    if (password_found) {
        std::cout << "The password is: " << password << std::endl;
    }
    delete[] passwordArray;
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

CPBreaker::~CPBreaker()
{
    delete[] char_codes;
}