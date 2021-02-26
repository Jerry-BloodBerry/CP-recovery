#pragma once
#ifndef PASSWORD_H_
#define PASSWORD_H_
#include <string>
#include "Charset.h"

class Password
{
	private:
		Charset charset;
		int* combination_array;
		bool increment_combination_array();
		std::string build_password_string_from_array();
	public:
		unsigned int length;
		std::string value;
		Password(unsigned int length, Charset charset);
		~Password();
		std::string toString() { return build_password_string_from_array(); }
		void set_combination(int start_char);
		bool next();
};
#endif

