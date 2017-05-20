#ifndef _H_BRAXNUMBER_H_
#define _H_BRAXNUMBER_H_

#include <unordered_map>

constexpr int BRAX_BASE = 8;

const std::unordered_map<char,int> brax_character_value = {
    { 'b', 0 },
    { 'r', 1 },
    { 'a', 2 },
    { 'x', 3 },
    { 'B', 4 },
    { 'R', 5 },
    { 'A', 6 },
    { 'X', 7 }
};

int braxCharacterToInteger(char c);
int braxNumberToInteger(const std::string& brax_number);

#endif
