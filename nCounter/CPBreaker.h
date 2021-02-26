#pragma once
#ifndef CPBREAKER_H_
#define CPBREAKER_H_
#include <string>
#include <iostream>
#include <botan/bcrypt.h>
#include <botan/argon2.h>
#include <botan/sha160.h>
#include "Password.h"
#include "Charset.h"

class CPBreaker
{
	private:
		bool password_found = false;
		Charset charset;
	public:
		std::string hash;
		unsigned int password_length;
		CPBreaker(Charset charset, unsigned int password_length, std::string hash_string);
		~CPBreaker();
		bool password_was_found() { return password_found; };
		void check_password_range(unsigned int startChar, unsigned int endChar);
};
#endif
