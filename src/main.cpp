#include "lexer.hpp"
#include <iostream>
#include <fstream>

std::ostream& operator<<(std::ostream& os, const Tokens& token)
{
    switch (token) {
    case WORD:
        os << "WORD";
        break;
    case END_OF_STREAM:
        os << "END_OF_STREAM";
    }
    return os;
};
        

std::ostream& operator<<(std::ostream& os, const Group& group)
{
    os << group.lexeme << ' ' << group.token;
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
    std::ifstream source_file("bite.bt");
    Lexer lexer(source_file);
    lexer.run();
    std::cout << lexer.getGroups() << std::endl;
    return 0;
}
