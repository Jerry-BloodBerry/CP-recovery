#include <iostream>
#include <math.h>
#include <chrono>
#include <vector>
#include <thread>
#include <math.h>
#include <botan/bcrypt.h>
#include "CPBreaker.h"

using std::cout;
using std::string;
using namespace Botan;

const int NUM_THREADS = std::thread::hardware_concurrency();
bool passwordWasFound = false;

void printHelp()
{
    cout << "#### ####    #### ####   #   #### #  # #### ####" << std::endl;
    cout << "#    #  #    #    #  #  # #  #    # #  #    #  #" << std::endl;
    cout << "#    ####    #    #### ##### #    ##   #### ####" << std::endl;
    cout << "#    #       #    # #  #   # #    # #  #    # # " << std::endl;
    cout << "#### #       #### #  # #   # #### #  # #### #  #" << std::endl << std::endl;
    cout << "CP CRACKER is a concurrent bruteforce password cracking utility." << std::endl;
    cout << "This version is meant to run on CPU only." << std::endl;
    cout << "Currently the following hash functions are supported: Bcrypt" << std::endl << std::endl;
    cout << "Options:" << std::endl;
    cout << "-help => display help" << std::endl;
    cout << "-l => password length range, ex. 4-8 (m)" << std::endl;
    cout << "-f => path to file containing the hash (m)" << std::endl;
    cout << "-c => characters to include in character set, ex. aA0 (m)" << std::endl;
    cout << "Character classes:" << std::endl;
    cout << "    a => small letters" << std::endl;
    cout << "    A => capital letters" << std::endl;
    cout << "    0 => numbers [0-9]" << std::endl;
    cout << "    % => special symbols (!, @, #, $, ...)" << std::endl;
}

void handleUserInput(int argc, char* argv[])
{
    if (argc == 1 && argv[0] == "-help") {
        printHelp();
        exit(0);
    }
    else if (argc == 6) {
        for (int i = 0; i < argc; i+=2) {
            string flag = argv[i];
            if (flag == "-c") {
                string charactersString = argv[i + 1];
                int charactersStringLength = charactersString.length();
                if (charactersStringLength == 0 || charactersStringLength > 4) {
                    cout << "At least one and no more than four character classes should be specified." << std::endl;
                    exit(2);
                }
                for (int j = 0; j < charactersStringLength; ++j) {
                    if (charactersString[i] != 'a' || charactersString[i] != 'A' || charactersString[i] != '0' || charactersString[i] != '%') {
                        cout << "Error processing input. Invalid character class " << charactersString[i] << " was given." << std::endl;
                        exit(3);
                    }
                }
            }
            else if (flag == "-l") {

            }
            else if (flag == "-f") {

            }
        }
    }
    else {
        cout << "Invalid parameter number. Refer to -help on how to form correct input arguments." << std::endl;
        exit(1);
    }
}

int main(int argc, char* argv[])
{
    const int CHAR_COUNT = 26;
    const int PASSWORD_LENGTH = 4;
    const long long COMBINATIONS = pow(CHAR_COUNT, PASSWORD_LENGTH);
    const int STEP = ceil(CHAR_COUNT / (float)NUM_THREADS);

    int charsLeft = CHAR_COUNT;
    int* char_codes = CPBreaker::build_characters_array("a");
    CPBreaker * password_breaker_ptr = new CPBreaker(char_codes, CHAR_COUNT, PASSWORD_LENGTH, "$2b$10$fMGdxFw5m19lT7q5pVhio.vpXKL4NAgGLPR4u1IwbY4JFsqIt1YD.");

    auto time_start = std::chrono::high_resolution_clock::now();
    std::vector<std::thread> my_threads;
    for (int i = 0; i < NUM_THREADS && charsLeft > 0; ++i) {
        my_threads.push_back(std::thread{ &CPBreaker::check_password_range, password_breaker_ptr, i * STEP, i * STEP + fmin(STEP, charsLeft) });
        charsLeft -= fmin(STEP, charsLeft);
    }
    cout << "All threads initialized\n";
    cout << "Proceeding to check " << COMBINATIONS << " combinations" << std::endl;
    for (std::thread& thr : my_threads) {
        thr.join();
    }
    auto time_end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_start).count();
    cout << "Execution time: " << duration << "ms" << std::endl;
    delete[] char_codes;
    return 0;
}
