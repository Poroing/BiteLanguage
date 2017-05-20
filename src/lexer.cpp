#include "lexer.hpp"

#include "tokens.hpp"
#include "stringtable.hpp"

#include <algorithm>
#include <boost/lexical_cast.hpp>
#include <cctype>
#include <iostream>
#include <string>

const std::vector<char> Lexer::valid_characters = { 'b', 'r', 'a', 'x' };

//TODO: implement a unordered_map wrapper that fits my need and is readable
const StringTable<Token> Lexer::lexemToken = {
    { "BRax", NUMBER_START },
    { "BRAx", ADD_OP },
    { "BRAW", SUB_OP },
    { "brax", RIGHT_PAREN },
    { "Brax", LEFT_PAREN }
};

Lexer::Lexer(std::istream& program_stream):
    program_stream(program_stream.rdbuf()),
    current_line(1), current_column(0)
{}

void Lexer::run()
{
    while (!isAtEndOfStream()) {
        addNextGroup();
    }
}

bool Lexer::isAtEndOfStream() const
{
    return current_character == EOF_CHARACTER;
}

Group Lexer::getNextGroup()
{
    addNextGroup();
    return groups.back();
}

void Lexer::addNextGroup()
{
    resetCurrentLexeme();
    getNextNonSpaceCharacter();
    if (current_character == EOF_CHARACTER) {
        addEndOfStream();
    } else {
        addWord();
    }
}

void Lexer::resetCurrentLexeme()
{
    current_lexeme.clear();
}

void Lexer::addEndOfStream()
{
    addCurrentCharacter();
    addCurrentGroup(END_OF_STREAM);
}

void Lexer::addWord()
{
    getLexeme();
    addCurrentGroup(getCurrentLexemeToken());
}

void Lexer::getNextNonSpaceCharacter()
{
    getNextCharacter();
    while (isCurrentCharacterBlank()) {
        getNextCharacter();
    }
}

void Lexer::getLexeme()
{
    while (!isCurrentCharacterBlank()) {
        checkCurrentCharacterValidity();
        addCurrentCharacter();
        getNextCharacter();
    }
}

void Lexer::addCurrentCharacter()
{
    current_lexeme.push_back(current_character);
}

void Lexer::getNextCharacter()
{
    current_character = program_stream.get();
    updatePosition();
}

void Lexer::updatePosition()
{
    if (current_character == '\n') {
        ++current_line;
        current_column = 0;
    } else {
        ++current_column;
    }
}

void Lexer::addCurrentGroup(Token token)
{
    checkTokenValidity(token);
    groups.emplace_back(current_lexeme, token);
}

void Lexer::checkTokenValidity(Token token) const
{
    if (token == UNKNOWN)
        throw std::runtime_error(
            std::string("Unknown Lexeme at: " + getPositionString()));
}

Token Lexer::getCurrentLexemeToken() const
{
    if (current_lexeme.size() > 4)
        //Lexeme with more than 4 character can only be NUMBER
        return (isCurrentLexemeTokenNumber())? NUMBER : UNKNOWN;
    return Lexer::lexemToken.getValueAssociatedOrdefault(current_lexeme,
        UNKNOWN);
}

bool Lexer::isCurrentLexemeTokenNumber() const
{
    return Lexer::lexemToken.isKeyAssociatedToValue(
        getLexemeRelevantPart(current_lexeme), NUMBER);
}

std::string Lexer::getLexemeRelevantPart(const std::string& lexeme) const
{
    return std::string(lexeme, 0, Lexer::WORD_LENGTH);
}

void Lexer::checkCurrentCharacterValidity() const
{
    if (!Lexer::isValidCharacter(current_character))
        throw std::runtime_error(
            std::string("Invalid Character: ")
            + '\'' + current_character + '\''
            + " at " + getPositionString());
}

std::string Lexer::getPositionString() const
{
    return boost::lexical_cast<std::string>(current_line)
        + ':'
        + boost::lexical_cast<std::string>(current_column);
}

bool Lexer::isValidCharacter(char c)
{
    return std::find(Lexer::valid_characters.begin(),
        Lexer::valid_characters.end(),
        std::tolower(c, std::locale::classic()))
    != Lexer::valid_characters.end();
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

