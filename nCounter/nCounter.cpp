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


string buildPasswordStringFromArray(int* passwordArray, int passwordLength, int* charCodes) {
    string password;
    password.reserve(passwordLength);
    for (int i = 0; i < passwordLength; ++i) {
        password += (char)(charCodes[passwordArray[i]]);
    }
    return password;
}

bool incrementPasswordArray(int* passwordArray, int passwordLength, int charCount) {
    bool incrementationSucceeded = false;
    if (passwordArray[passwordLength - 1] + 1 != charCount) {
        ++passwordArray[passwordLength - 1];
        incrementationSucceeded = true;
    }
    else {
        int i = 0;
        while (i != passwordLength && passwordArray[passwordLength - 1 - i] + 1 == charCount) {
            passwordArray[passwordLength - 1 - i] = 0;
            ++i;
        }
        if (i != passwordLength) {
            ++passwordArray[passwordLength - 1 - i];
            incrementationSucceeded = true;
        }
    }
    return incrementationSucceeded;
}

void countToN(int passwordLength, int* charCodes, int charCount, int startChar, int endChar) {
    int* passwordArray = new int[passwordLength];
    passwordArray[0] = startChar;
    for (int i = 1; i < passwordLength; ++i) {
        passwordArray[i] = 0;
    }
    bool incrementationSucceeded = true;
    while (incrementationSucceeded && !passwordWasFound) {
        string password = buildPasswordStringFromArray(passwordArray, passwordLength, charCodes);
        bool result = check_bcrypt(password, "$2b$10$MOV.u1EbxIonlQ42OY53Mez/0WdAnJ.aQO1CUG7dBIrfq4SLyg3wS");
        if (result) {
            cout << "The password is: " << password << std::endl;
            passwordWasFound = true;
        }
        incrementationSucceeded = incrementPasswordArray(passwordArray, passwordLength, charCount) && (passwordArray[0] != (char)endChar);
    }
    delete[] passwordArray;
}

string buildCharactersString(string symbols) {
    bool containsSmallLetters = symbols.find('a') != std::string::npos;
    bool containsBigLetters = symbols.find('A') != std::string::npos;
    bool containsNumbers = symbols.find('0') != std::string::npos;
    bool containsSymbols = symbols.find('%') != std::string::npos;

    string charactersString = "";
    if (containsSmallLetters) {
        for (int i = 97; i < 123; ++i) {
            charactersString += (char)i;
        }
    }
    if (containsBigLetters) {
        for (int i = 65; i < 91; ++i) {
            charactersString += (char)i;
        }
    }
    if (containsNumbers) {
        for (int i = 48; i < 58; ++i) {
            charactersString += (char)i;
        }
    }
    if (containsSymbols) {
        for (int i = 32; i < 48; ++i) {
            charactersString += (char)i;
        }
        for (int i = 58; i < 65; ++i) {
            charactersString += (char)i;
        }
        for (int i = 91; i < 97; ++i) {
            charactersString += (char)i;
        }
        for (int i = 123; i < 127; ++i) {
            charactersString += (char)i;
        }
    }
    return charactersString;
}

int* buildCharactersArray(string symbols)
{
    string charactersString = buildCharactersString(symbols);
    int charactersCount = charactersString.length();
    int* characters = new int[charactersCount];
    for (int i = 0; i < charactersCount; ++i) {
        characters[i] = charactersString[i];
    }
    return characters;
}

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
    int* charCodes = buildCharactersArray("aA0%");

    auto time_start = std::chrono::high_resolution_clock::now();
    std::vector<std::thread> my_threads;
    for (int i = 0; i < NUM_THREADS && charsLeft > 0; ++i) {
        my_threads.push_back(std::thread{ countToN, PASSWORD_LENGTH, charCodes, CHAR_COUNT, i * STEP, i * STEP + fmin(STEP, charsLeft) });
        charsLeft -= fmin(STEP, charsLeft);
    }
    cout << "All threads initialized\n";
    cout << "Proceeding to check " << COMBINATIONS << " combinations";
    for (std::thread& thr : my_threads) {
        thr.join();
    }
    auto time_end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_start).count();
    cout << "Execution time: " << duration << "ms" << std::endl;
    delete[] charCodes;
    return 0;
}
