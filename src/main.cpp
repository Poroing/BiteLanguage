#include "lexer.hpp"
#include "stringtable.hpp"
#include <iostream>
#include <fstream>

std::ostream& operator<<(std::ostream& os, const Token& token)
{
    switch (token) {
    case WORD:
        os << "WORD";
        break;
    case END_OF_STREAM:
        os << "END_OF_STREAM";
        break;
    case NUMBER_START:
        os << "NUMBER_START";
        break;
    case NUMBER:
        os << "NUMBER";
        break;
    case LEFT_PAREN:
        os << "LEFT_PAREN";
        break;
    case RIGHT_PAREN:
        os << "RIGHT_PAREN";
        break;
    case ADD_OP:
        os << "ADD_OP";
        break;
    case SUB_OP:
        os << "SUB_OP";
        break;
    default:
        os << "UNKNOWN";
    }
    return os;
};


std::ostream& operator<<(std::ostream& os, const Group& group)
{
    os << '\'' << group.lexeme << '\'' << ' ' << group.token;
    return os;
};

std::ostream& operator<<(std::ostream& os, const std::vector<Group>& groups)
{
    os << '[';
    for (auto it = groups.begin(); it < groups.end() - 1; ++it) {
        os << *it;
        os << ',';
    }
    os << groups.back();
    os << ']';
    return os;
};

int main(int argc, char* argv[])
{
    std::ifstream file("file.bt");
    Lexer lx(file);
    while (!lx.isAtEndOfStream()) {
        std::cout << lx.getNextGroup() << std::endl;
    }

    return 0;
}
