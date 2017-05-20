#include "number.hpp"

int braxCharacterToInteger(char c)
{
    return brax_character_value.at(c);
}

int braxNumberToInteger(const std::string& brax_number)
{
    int number = 0;
    int current_character_index = 0;
    auto current_character = brax_number.end() - 1;
    while (current_character >= brax_number.begin()) {
        number += BRAX_BASE * current_character_index;
        ++current_character_index;
        --current_character;
    }
    return number;
}
    
    
