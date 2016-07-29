#ifndef _H_BITENUMBER_H_
#define _H_BITENUMBER_H_

#include <unordered_map>

constexpr int BITE_BASE = 8;

const std::unordered_map<char,int> bite_character_value = {
    { 'b', 0 },
    { 'i', 1 },
    { 't', 2 },
    { 'e', 3 },
    { 'B', 4 },
    { 'I', 5 },
    { 'T', 6 },
    { 'E', 7 }
};

int biteCharacterToInteger(char c);
int biteNumberToInteger(const std::string& bite_number);

#endif
