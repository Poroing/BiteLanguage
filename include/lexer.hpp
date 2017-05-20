#ifndef _H_BRAX_LEXER_H_
#define _H_BRAX_LEXER_H_

#include "tokens.hpp"
#include "stringtable.hpp"

#include <vector>
#include <string>
#include <istream>
#include <unordered_map>
#include <iterator>

struct Group {
    Group(const std::string& lexeme, Token token);

    std::string lexeme;
    Token token;
};


class Lexer
{
    constexpr static auto EOF_CHARACTER = std::istream::traits_type::eof();
    constexpr static int WORD_LENGTH = 4;

    const static std::vector<char> valid_characters;
    const static StringTable<Token> lexemToken;

    static bool isValidCharacter(char c);

public:
    Lexer(std::istream& program_stream);

    void run();

    std::string getPositionString() const;
    const std::vector<Group>& getGroups() const;
    Group getNextGroup();

    bool isAtEndOfStream() const;

private:
    bool isCurrentCharacterBlank() const;
    bool isCurrentLexemeTokenNumber() const;

    void checkCurrentCharacterValidity() const;
    void checkTokenValidity(Token token) const;

    void getNextCharacter();
    void getNextNonSpaceCharacter();
    void getLexeme();

    Token getCurrentLexemeToken() const;
    std::string getLexemeRelevantPart(const std::string& lexeme) const;

    void updatePosition();
    void resetCurrentLexeme();

    void addCurrentCharacter();
    void addNextGroup();
    void addCurrentGroup(Token token);
    void addEndOfStream();
    void addWord();

    char current_character;
    std::string current_lexeme;
    int current_line;
    int current_column;

    std::vector<Group> groups;

    std::istream program_stream;
};

#endif
