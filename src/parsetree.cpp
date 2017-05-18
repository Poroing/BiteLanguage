#include "parsetree.hpp"
#include <iterator>

ParseTree::ParseTree(Token token):
    token(token)
{};

template<typename GroupIterator>
ParseTree buildParseTree(GroupIterator& groups_begin, GroupIterator& groups_end)
{
    if (groups_begin >= groups_end)
        throw std::runtime_error("There is no groups to parse");

    
        
}

template<typename GroupIterator>
ParseTree parseArithmeticExpression(GroupIterator& groups_begin, GroupIterator& groups_end)
{
    
}

template<typename GroupIterator>
ParseTree getParseArithmeticExpressionInfo(
    GroupIterator& groups_begin,
    GroupIterator& groups_end)
{
    ParseTree parsing_info;
    GroupIterator current_position = groups_begin;

    if (groups_begin->token == LEFT_PAREN)
    {
        ++current_position;

            getParseArithmeticExpressionInfo(current_position, groups_end);
        if (sub_parsing_info.end->token != RIGHT_PAREN)
        {
            throw std::invalid_argument("Missing right parenthese");
        }
        
        ++current_position;
        if (
    }
}

template<typename GroupIterator>
ParseInfo<GroupIterator> getParseOperatorInfo(
    GroupIterator& groups_begin,
    GroupIterator& groups_end)
{
    ParseInfo<GroupIterator> parsing_info;
    if (parse
}
