#pragma once
#ifndef CPBREAKER_H_
#define CPBREAKER_H_
#include <string>
#include <iostream>
#include <functional>
#include <botan/bcrypt.h>
#include <botan/argon2.h>
#include <botan/sha160.h>
#include "Password.h"
#include "Charset.h"
#include <stdexcept>

class CPBreaker
{
	private:
		bool password_found = false;
		Charset charset;
		std::function<bool(std::string)> password_matches_hash;
		bool check_bcrypt_hash(std::string password);
		bool check_argon_hash(std::string password);
	public:
		enum HASHING_FUNCTIONS {
			BCRYPT = 0,
			ARGON = 1
		};
		std::string hash;
		unsigned int password_length;
		CPBreaker(Charset charset, unsigned int password_length, std::string hash_string, int hashing_function);
		~CPBreaker();
		bool password_was_found() { return password_found; };
		void check_password_range(unsigned int startChar, unsigned int endChar);
};
#endif
