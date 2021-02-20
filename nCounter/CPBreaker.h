#pragma once
#ifndef CPBREAKER_H_
#define CPBREAKER_H_
#include <string>
#include <iostream>
#include <botan/bcrypt.h>

class CPBreaker
{
	private:
		int* char_codes;
		bool password_found = false;
		int password_length;
		int char_set_count;
		static std::string build_characters_string(std::string symbols);
		bool increment_password_array(int *password_array);
		std::string build_password_string_from_array(int* passwordArray);
	public:
		std::string hash;
		CPBreaker(int* characters, int char_count, int pass_length, std::string hash_string);
		~CPBreaker();
		static int* build_characters_array(std::string symbols) {
			std::string characters_string = build_characters_string(symbols);
			int characters_count = characters_string.length();
			int* characters = new int[characters_count];
			for (int i = 0; i < characters_count; ++i) {
				characters[i] = characters_string[i];
			}
			return characters;
		}
		void check_password_range(int startChar, int endChar)
		{
			int* passwordArray = new int[password_length];
			passwordArray[0] = startChar;
			for (int i = 1; i < password_length; ++i) {
				passwordArray[i] = 0;
			}
			bool incrementationSucceeded = true;
			while (incrementationSucceeded && !password_found) {
				std::string password = build_password_string_from_array(passwordArray);
				bool result = Botan::check_bcrypt(password, hash);
				if (result) {
					std::cout << "The password is: " << password << std::endl;
					password_found = true;
				}
				incrementationSucceeded = increment_password_array(passwordArray) && (passwordArray[0] != (char)endChar);
			}
			delete[] passwordArray;
		}
};
#endif
