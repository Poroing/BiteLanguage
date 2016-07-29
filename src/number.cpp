#include "number.hpp"

int biteCharacterToInteger(char c)
{
    return bite_character_value.at(c);
}

int biteNumberToInteger(const std::string& bite_number)
{
    int number = 0;
    int current_character_index = 0;
    auto current_character = bite_number.end() - 1;
    while (current_character >= bite_number.begin()) {
        number += BITE_BASE * current_character_index;
        ++current_character_index;
        --current_character;
    }
    return number;
}
    
    
