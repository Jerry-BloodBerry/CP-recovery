#pragma once
#ifndef CHARSET_H_
#define CHARSET_H_
#include <string>
#include <stdexcept>
class Charset
{
	private:
		int* charset;
		unsigned int charset_count;
		int* charset_from_symbols(std::string symbols);
		void appendSmallLetters(int* &charset, int currentIndex);
		void appendBigLetters(int* &charset, int currentIndex);
		void appendNumbers(int* &charset, int currentIndex);
		void appendSpecialCharacters(int* &charset, int currentIndex);
	public:
		Charset(std::string symbols);
		Charset(int* charset, unsigned int charset_count);
		Charset();
		~Charset();
		static int get_size_for_symbols(std::string symbols);
		int* get_charset() { return this->charset; };
		unsigned int get_count() { return this->charset_count; };
};
#endif
