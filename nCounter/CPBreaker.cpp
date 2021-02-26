#include "CPBreaker.h"
#include <string>

CPBreaker::CPBreaker(Charset charset, unsigned int password_length, std::string hash_string)
{
    this->charset = charset;
    this->password_length = password_length;
    this->hash = hash_string;
}

void CPBreaker::check_password_range(unsigned int start_char_code, unsigned int end_char_code)
{
    Password password = Password(this->password_length, this->charset);
    password.set_combination(start_char_code);
    bool incrementationSucceeded = true;
    std::string passwordString = "";
    while (incrementationSucceeded && !password_found) {
        passwordString = password.toString();
        bool result = Botan::argon2_check_pwhash(passwordString.c_str(), password_length, this->hash);
        if (result) {
            password_found = true;
            std::cout << "The password is: " << password.toString() << std::endl;
        }
        incrementationSucceeded = password.next() && (passwordString[0] != (char)end_char_code);
    }
}

CPBreaker::~CPBreaker()
{
}