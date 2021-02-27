#include <iostream>
#include <math.h>
#include <chrono>
#include <vector>
#include <thread>
#include <math.h>
#include <regex>
#include <fstream>
#include "CPBreaker.h"
#include "Charset.h"


using std::cout;
using std::string;

const unsigned int NUM_THREADS = std::thread::hardware_concurrency() * 2;
Charset g_charset;
unsigned int g_password_range_low;
unsigned int g_password_range_high;
string g_hash;

void printHelp()
{
    cout << "#### ####    ####  ####  ####  ####  #   #  ####  ####  #   #" << std::endl;
    cout << "#    #  #    #  #  #     #     #  #  #   #  #     #  #   # #" << std::endl;
    cout << "#    ####    ####  ####  #     #  #  #   #  ####  ####    #" << std::endl;
    cout << "#    #       # #   #     #     #  #   # #   #     # #     #" << std::endl;
    cout << "#### #       #  #  ####  ####  ####    #    ####  #  #    #" << std::endl << std::endl;
    cout << "CP RECOVERY is a concurrent bruteforce password cracking utility." << std::endl;
    cout << "This version is meant to run on CPU only." << std::endl;
    cout << "Currently the following hash functions are supported: Argon" << std::endl << std::endl;
    cout << "Options:" << std::endl;
    cout << "-help => display help" << std::endl;
    cout << "-l => dash separated password length range, ex. 4-8 (m)" << std::endl;
    cout << "-f => path to file containing the hash (m)" << std::endl;
    cout << "-c => characters to include in character set, ex. aA (m)" << std::endl;
    cout << "Character classes:" << std::endl;
    cout << "    a => small letters" << std::endl;
    cout << "    A => capital letters" << std::endl;
    cout << "    0 => numbers [0-9]" << std::endl;
    cout << "    % => special symbols (!, @, #, $, ...)" << std::endl;
}

bool string_contains_duplicate_characters(string characters)
{
    int characters_str_length = characters.length();
    for (int i = 0; i < characters_str_length; ++i) {
        for (int j = i + 1; j < characters_str_length; ++j) {
            if (characters[j] == characters[i]) {
                return true;
            }
        }
    }
    return false;
}

bool validate_characters_input(string characters_string) {
    std::regex pattern("^[aA0%]{1,4}$");
    return std::regex_match(characters_string.begin(), characters_string.end(), pattern) &&
           !string_contains_duplicate_characters(characters_string);
}

bool validate_password_range_input(string password_range_string)
{
    std::regex pattern("^(\\d+)-(\\d+)$");
    std::smatch matches;
    if (std::regex_search(password_range_string, matches, pattern)) {
        int range_low = std::stoi(matches[1].str());
        int range_high = std::stoi(matches[2].str());
        return range_low > 0 && range_high >= range_low;
    }
    return false;
}

void handleUserInput(int argc, char* argv[])
{
    if (argc == 2 && string(argv[1]) == "-help") {
        printHelp();
        exit(0);
    }
    else if (argc == 7) {
        bool characters_initialized = false;
        bool password_range_initialized = false;
        bool hash_obtained_from_file = false;
        for (int i = 1; i < argc; i+=2) {
            string flag = string(argv[i]);
            if (flag == "-c") {
                string characters_string = string(argv[i + 1]);
                if (validate_characters_input(characters_string)) {
                    g_charset = Charset(characters_string);
                    characters_initialized = true;
                }
                else {
                    cout << "Invalid character classes string. Please check your input or refer to -help for clues." << std::endl;
                    exit(2);
                }
            }
            else if (flag == "-l") {
                string password_range_string = string(argv[i + 1]);
                if (validate_password_range_input(password_range_string)) {
                    std::regex pattern("^(\\d+)-(\\d+)$");
                    std::smatch matches;
                    std::regex_search(password_range_string, matches, pattern);
                    g_password_range_low = (int)std::stoi(matches[1].str());
                    g_password_range_high = (int)std::stoi(matches[2].str());
                    password_range_initialized = true;
                }
                else {
                    cout << "Invalid password range. Please check your input or refer to -help for clues." << std::endl;
                    exit(3);
                }
            }
            else if (flag == "-f") {
                string path_to_file = string(argv[i + 1]);
                std::ifstream file_with_hash(path_to_file);
                if (file_with_hash.good()) {
                    std::getline(file_with_hash, g_hash);
                    if (g_hash.length() > 0) {
                        hash_obtained_from_file = true;
                    }
                    else {
                        cout << "File does not contain a valid hash. Hash must be at least 1 character long";
                        exit(4);
                    }
                }
                else {
                    cout << "Cannot open the hash file. Please make sure you've given the correct path to file." << std::endl;
                    exit(4);
                }
            }
            else {
                cout << "Unsupported flag " << flag << ". Please refer to -help for list of supported flags." << std::endl;
                exit(5);
            }
        }
        if (!characters_initialized) {
            cout << "Error: character flag missing.";
            exit(6);
        }
        if (!password_range_initialized) {
            cout << "Error: password range flag missing.";
            exit(6);
        }
        if (!hash_obtained_from_file) {
            cout << "Error: file flag missing.";
            exit(6);
        }
    }
    else {
        cout << "Invalid parameter number. Refer to -help on how to form correct input arguments." << std::endl;
        exit(1);
    }
}

int main(int argc, char* argv[])
{
    handleUserInput(argc, argv);
    unsigned int STEP = ceil(g_charset.get_count() / (float)NUM_THREADS);
    auto time_start = std::chrono::high_resolution_clock::now();
    CPBreaker* password_breaker_ptr = new CPBreaker(g_charset, g_password_range_low, g_hash, CPBreaker::ARGON);
    for (int password_length = g_password_range_low; password_length <= g_password_range_high; ++password_length)
    {
        password_breaker_ptr->password_length = password_length;
        int charsLeft = g_charset.get_count();
        unsigned long long combinations = pow(g_charset.get_count(), password_length);
        std::vector<std::thread> my_threads;
        cout << "Now checking passwords of length: " << password_length << std::endl;
        cout << "Proceeding to check " << combinations << " combinations" << std::endl;
        for (int i = 0; i < NUM_THREADS && charsLeft > 0; ++i) {
            my_threads.push_back(std::thread{ &CPBreaker::check_password_range, password_breaker_ptr, i * STEP, i * STEP + fmin(STEP, charsLeft) });
            charsLeft -= fmin(STEP, charsLeft);
        }
        for (std::thread& thr : my_threads) {
            thr.join();
        }
        my_threads.clear();
        if (password_breaker_ptr->password_was_found()) {
            break;
        }
        cout << "No luck so far." << std::endl << std::endl;
    }  
    auto time_end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_start).count();
    cout << "Execution time: " << duration << "ms" << std::endl;
    return 0;
}
