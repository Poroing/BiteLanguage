#ifndef _H_BITE_LEXER_H_
#define _H_BITE_LEXER_H_

#include <vector>
#include <string>
#include <istream>

enum Token {
    WORD,
    END_OF_STREAM
};

struct Group {
    Group(const std::string& lexeme, Token token);

    std::string lexeme;
    Token token;
};


class Lexer
{
    constexpr static auto EOF_CHARACTER = std::istream::traits_type::eof();

    const static std::vector<char> valid_characters;
    static bool isValidCharacter(char c);

public:
    Lexer(std::istream& program_stream);

    void run();
    const std::vector<Group>& getGroups() const;

private:
    bool isAtEndOfStream() const;
    bool isCurrentCharacterBlank() const;
    void checkCurrentCharacterValidity() const;

    void getNextCharacter();
    void getNextNonSpaceCharacter();

    void addCurrentCharacter();
    void addNextGroup();
    void addGroup(const std::string& lexeme, Token token);
    void addEndOfStream();
    void addWord();

    char current_character;
    std::string current_lexeme;
    std::vector<Group> groups;
    std::istream program_stream;
};

#endif
