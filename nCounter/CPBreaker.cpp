#include "CPBreaker.h"
#include <string>

CPBreaker::CPBreaker(Charset charset, unsigned int password_length, std::string hash_string, int hashing_function)
{
    this->charset = charset;
    this->password_length = password_length;
    this->hash = hash_string;
    switch (hashing_function) {
        case HASHING_FUNCTIONS::ARGON:
            this->password_matches_hash = std::bind(&CPBreaker::check_argon_hash, this, std::placeholders::_1);
            break;
        case HASHING_FUNCTIONS::BCRYPT:
            this->password_matches_hash = std::bind(&CPBreaker::check_bcrypt_hash, this, std::placeholders::_1);
            break;
        default:
            throw std::runtime_error("Undefined hashing function passed to CPBreaker::CPBreaker");
    }
}

bool CPBreaker::check_argon_hash(std::string password)
{
    return Botan::argon2_check_pwhash(password.c_str(), this->password_length, this->hash);
}

bool CPBreaker::check_bcrypt_hash(std::string password)
{
    return Botan::check_bcrypt(password, this->hash);
}

void CPBreaker::check_password_range(unsigned int start_char_code, unsigned int end_char_code)
{
    Password password = Password(this->password_length, this->charset);
    password.set_combination(start_char_code);
    bool incrementationSucceeded = true;
    std::string passwordString = "";
    while (incrementationSucceeded && !password_found) {
        passwordString = password.toString();
        bool result = password_matches_hash(passwordString);
        if (result) {
            password_found = true;
            std::cout << "The password is: " << passwordString << std::endl;
        }
        incrementationSucceeded = password.next() && (passwordString[0] != (char)end_char_code);
    }
}

CPBreaker::~CPBreaker()
{
}