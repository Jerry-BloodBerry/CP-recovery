#pragma once
#ifndef CPBREAKER_H_
#define CPBREAKER_H_
#include <string>
#include <iostream>
#include <botan/bcrypt.h>
#include <botan/sha160.h>

class CPBreaker
{
	private:
		int* char_codes;
		bool password_found = false;
		unsigned int char_set_count;
		static std::string build_characters_string(std::string symbols);
		bool increment_password_array(int* password_array);
		std::string build_password_string_from_array(int* passwordArray);
	public:
		unsigned int password_length;
		std::string hash;
		CPBreaker(int* characters, unsigned int char_count, unsigned int pass_length, std::string hash_string);
		~CPBreaker();
		bool password_was_found() { return password_found; };
		static int* build_characters_array(std::string symbols);
		static int get_array_size_for_symbols(std::string symbols);
		void check_password_range(unsigned int startChar, unsigned int endChar);
};
#endif
