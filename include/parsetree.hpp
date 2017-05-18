#ifndef _H_BITEPARSETREE_H_
#define _H_BITEPARSETREE_H_

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
    ParseTreeBase(Token token);
};

template<Token token>
class ParseTreeTerminal : public ParseTreeBase
{
public:
    ParseTreeTerminal():
        ParseTreeBase(token)
    {
        static_assert(isTerminal(token), "The token must be terminal");
    };
};

template<Token token>
class ParseTreeNonTerminal : public ParseTreeBase
{
public:
    ParseTreeNonTerminal():
        ParseTreeBase(token)
    {
        static_assert(!isTerminal(token), "The token must be non terminal");
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
ParseTreeNonTerminal<ARITH_EXPR> buildParseTree(GroupIterator& groups_begin, GroupIterator& groups_end);

template<typename GroupIterator>
ParseTreeNonTerminal<ARITH_EXPR>
parseArithmeticExpression(GroupIterator& groups_begin, GroupIterator& groups_end);

#endif

