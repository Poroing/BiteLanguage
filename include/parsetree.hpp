#ifndef _H_BRAXPARSETREE_H_
#define _H_BRAXPARSETREE_H_

#include <vector>
#include <memory>

#include "lexer.hpp"
#include "tokens.hpp"

constexpr bool isTerminal(Token token)
{
    switch(token)
    {
    case NUMBER:
    case ADD_OP:
    case SUB_OP:
    case LEFT_PAREN:
    case RIGHT_PAREN:
        return true;
    default:
        return false;
    }
}

bool isOperatorBeginning(Token token)
{
    switch(token)
    {
    case ADD_OP:
    case SUB_OP:
        return true;
    default:
        return false;
    }
}

class ParseTreeBase
{
public:
    const Token token;

protected:
    ParseTreeBase(Token token):
        token(token)
    {};
};

template<Token parse_tree_token>
class ParseTreeTerminal : public ParseTreeBase
{
    static_assert(isTerminal(parse_tree_token), "The token must be terminal");

public:
    ParseTreeTerminal():
        ParseTreeBase(token)
    {};

    ParseTreeTerminal(ParseTreeTerminal<parse_tree_token>&& other):
        ParseTreeBase(token)
    {};
};

template<Token parse_tree_token>
class ParseTreeNonTerminal : public ParseTreeBase
{
    static_assert(!isTerminal(parse_tree_token), "The token must be non terminal");

public:
    ParseTreeNonTerminal():
        ParseTreeBase(parse_tree_token)
    {}

    ParseTreeNonTerminal(ParseTreeNonTerminal<parse_tree_token>&& other):
        ParseTreeBase(token),
        childs(std::move(other.childs))
    {}

    template<typename ParseTree>
    void addChild(ParseTree&& child)
    {
        static_assert(std::is_base_of<ParseTreeBase, ParseTree>::value,
            "The child must derive from ParseTreeBase");

        childs.push_back(std::unique_ptr<ParseTreeBase>(new ParseTree(std::move(child))));
    }

    template<typename ParseTree>
    void addChild(ParseTree child)
    {
        addChild(std::move(child));
    }

    std::vector<std::unique_ptr<ParseTreeBase> > childs;
};

template<typename GroupIterator>
struct ParseTreeAndEnd
{
    std::unique_ptr<ParseTreeBase> parse_tree_ptr;
    GroupIterator end;
};

template<typename GroupIterator>
ParseTreeNonTerminal<ARITH_EXPR>
buildParseTree(GroupIterator& groups_begin, GroupIterator& groups_end);
{
    if (groups_begin >= groups_end)
        throw std::runtime_error("There is no groups to parse");

    
        
}

template<typename T>
constexpr void assertTypeIsGroupIterator(T t)
{
    static_assert(isGroupIterator(t), "The type must be an iterator on groups");
}
    

template<typename GroupIterator>
ParseTreeAndEnd<GroupIterator>
parseArithmeticExpression(GroupIterator& groups_begin, GroupIterator& groups_end)
{
    assertTypeIsGroupIterator(groups_begin);

    ParseTreeAndEnd<GroupIterator> parsed_tree_and_end;
    parsed_tree_and_end.parse_tree_ptr =
        std::unique_ptr<ParseTreeBase>(new ParseTreeNonTerminal<ARITH_EXPR>);
    
    GroupIterator current_position = groups_begin;

    if (groups_begin->token == LEFT_PAREN)
    {
        ++current_position;

        ParseTreeAndEnd<GroupIterator> sous_parse_tree = 
            parseArithmeticExpression(current_position, groups_end);
        current_position = sous_parse_tree.end;

        if (current_position->token != RIGHT_PAREN)
        {
            throw std::invalid_argument("Missing right parenthese");
        }
        ++current_position;

        if (current_position == groups_end || !isOperatorBeginning(current_position->token))
        {
            parsed_tree_and_end.parse_tree_ptr = std::move(sous_parse_tree.parse_tree_ptr);
            parsed_tree_and_end.end = current_position;
            return parsed_tree_and_end;
        }

        sous_parse_tree = parseOperator(current_position, groups_end);
        
    }
}

template<typename GroupIterator>
ParseTreeAndEnd<GroupIterator>
parseOperator(GroupIterator& groups_begin, GroupIterator& groups_end)
{
    assertTypeIsGroupIterator(groups_begin);

    auto parsed_tree = std::unique_ptr<ParseTreeNonTerminal<OPERATOR>>(
        new ParseTreeNonTerminal<OPERATOR>);
    if (groups_begin->token == ADD_OP)
        parsed_tree->addChild(ParseTreeTerminal<ADD_OP>())
    

#endif

