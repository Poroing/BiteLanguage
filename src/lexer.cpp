#include "lexer.hpp"
#include <algorithm>
#include <cctype>
#include <iostream>

const std::vector<char> Lexer::valid_characters = { 'b', 'i', 't', 'e' };

Lexer::Lexer(std::istream& program_stream):
    program_stream(program_stream.rdbuf())
{}

void Lexer::run()
{
    while (!isAtEndOfStream()) {
        addNextGroup();
    }
}

void Lexer::addNextGroup()
{
    current_lexeme.clear();
    getNextNonSpaceCharacter();
    if (current_character == EOF_CHARACTER) {
        addEndOfStream();
    } else {
        addWord();
    }
}

void Lexer::addEndOfStream()
{
    addCurrentCharacter();
    groups.emplace_back(current_lexeme, END_OF_STREAM);
}

void Lexer::addWord()
{
    while (!isCurrentCharacterBlank()) {
        checkCurrentCharacterValidity();
        addCurrentCharacter();
        getNextCharacter();
    }
}

void Lexer::addGroup(const std::string& lexeme, Token token)
{
    groups.emplace_back(current_lexeme, WORD);
}

void Lexer::getNextNonSpaceCharacter()
{
    getNextCharacter();
    while (isCurrentCharacterBlank()) {
        getNextCharacter();
    }
}

void Lexer::getNextCharacter()
{
    current_character = program_stream.get();
}

void Lexer::addCurrentCharacter()
{
    current_lexeme.push_back(current_character);
}

void Lexer::checkCurrentCharacterValidity() const
{
    if (!Lexer::isValidCharacter(current_character))
        throw std::runtime_error(
            std::string("Invalid Character: '") + current_character + '\'');
}

bool Lexer::isValidCharacter(char c)
{
    return std::find(Lexer::valid_characters.begin(), Lexer::valid_characters.end(),
        std::tolower(c, std::locale::classic()))
    != Lexer::valid_characters.end();
}

bool Lexer::isAtEndOfStream() const
{
    return current_character == EOF_CHARACTER;
}

bool Lexer::isCurrentCharacterBlank() const
{
    return std::isspace(current_character);
}

const std::vector<Group>& Lexer::getGroups() const
{
    return groups;
}

Group::Group(const std::string& lexeme, Token token):
    lexeme(lexeme),
    token(token)
{};

