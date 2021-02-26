#include "Password.h"

Password::Password(unsigned int length, Charset charset)
{
	this->length = length;
	this->combination_array = new int[length];
    for (int i = 0; i < length; i++)
    {
        combination_array[i] = 0;
    }
    this->charset = charset;
}

bool Password::next()
{
    return increment_combination_array();
}

bool Password::increment_combination_array()
{
    bool incrementationSucceeded = false;
    int charset_count = this->charset.get_count();
    if (this->combination_array[this->length - 1] + 1 != charset_count) {
        ++this->combination_array[this->length - 1];
        incrementationSucceeded = true;
    }
    else {
        int i = 0;
        while (i != this->length && this->combination_array[this->length - 1 - i] + 1 == charset_count) {
            this->combination_array[this->length - 1 - i] = 0;
            ++i;
        }
        if (i != this->length) {
            ++this->combination_array[this->length - 1 - i];
            incrementationSucceeded = true;
        }
    }
    return incrementationSucceeded;
}

std::string Password::build_password_string_from_array()
{
    std::string password;
    password.reserve(this->length);
    int* char_codes = this->charset.get_charset();
    for (int i = 0; i < this->length; ++i) {
        password += (char)(char_codes[this->combination_array[i]]);
    }
    return password;
}

void Password::set_combination(int start_char)
{
    this->combination_array[0] = start_char;
    for (int i = 1; i < this->length; ++i)
    {
        this->combination_array[i] = 0;
    }
}

Password::~Password()
{
	delete[] combination_array;
}