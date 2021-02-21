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
		int password_length;
		int char_set_count;
		static std::string build_characters_string(std::string symbols);
		bool increment_password_array(int* password_array);
		std::string build_password_string_from_array(int* passwordArray);
	public:
		std::string hash;
		CPBreaker(int* characters, int char_count, int pass_length, std::string hash_string);
		~CPBreaker();
		static int* build_characters_array(std::string symbols);
		static int get_array_size_for_symbols(std::string symbols);
		void check_password_range(int startChar, int endChar);
};
#endif
