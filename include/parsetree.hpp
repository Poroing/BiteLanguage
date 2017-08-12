#ifndef _H_BRAXPARSETREE_H_
#define _H_BRAXPARSETREE_H_

#include <vector>
#include <memory>
#include <iterator>

#include "lexer.hpp"
#include "tokens.hpp"
#include "type.hpp"

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

constexpr bool isOperatorBeginning(Token token)
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

template<
    Token parse_tree_token,
    typename = std::enable_if_t<isTerminal(parse_tree_token)>>
class ParseTreeTerminal : public ParseTreeBase
{

public:
    ParseTreeTerminal():
        ParseTreeBase(token)
    {};

    ParseTreeTerminal(ParseTreeTerminal<parse_tree_token>&& other):
        ParseTreeBase(token)
    {};
};

template<
    Token parse_tree_token,
    typename = std::enable_if<!isTerminal(parse_tree_token)>>
class ParseTreeNonTerminal : public ParseTreeBase
{
public:
    ParseTreeNonTerminal():
        ParseTreeBase(parse_tree_token)
    {}

    ParseTreeNonTerminal(ParseTreeNonTerminal<parse_tree_token>&& other):
        ParseTreeBase(token),
        childs(std::move(other.childs))
    {}

    //We have to test if the type is an rvalue reference since we don't want
    //the reference to be forwarding but to be an rvalue reference
    template<
        typename ParseTree,
        typename = std::enable_if_t<conjunction_v<
            std::is_base_of<ParseTreeBase, ParseTree>,
            std::is_rvalue_reference<ParseTree>>>>
    void addChild(ParseTree&& child)
    {
        childs.push_back(
            std::unique_ptr<ParseTreeBase>(new ParseTree(std::move(child))));
    }

    template<
        typename ParseTree,
        typename = std::enable_if_t<
            std::is_base_of<ParseTreeBase, ParseTree>::value>>
    void addChild(ParseTree child)
    {
        addChild(std::move(child));
    }

    std::vector<std::unique_ptr<ParseTreeBase> > childs;
};


template<
    typename GroupIterator,
    typename = std::enable_if_t<isGroupIterator_v<GroupIterator>>>
struct ParseTreeAndEnd
{
    std::unique_ptr<ParseTreeBase> parse_tree_ptr;
    GroupIterator end;
};


//TODO: No need for this in C++17, wait for C++17
template<
    typename GroupIterator,
    typename = std::enable_if_t<isGroupIterator_v<GroupIterator>>>
auto makeParseTreeAndEnd(
    std::unique_ptr<ParseTreeBase> parse_tree_ptr,
    GroupIterator end)
{
    return ParseTreeAndEnd<GroupIterator>{parse_tree_ptr, end};
}

template<
    typename GroupIterator,
    typename = std::enable_if_t<isGroupIterator_v<GroupIterator>>>
ParseTreeNonTerminal<ARITH_EXPR>
buildParseTree(GroupIterator& groups_begin, GroupIterator& groups_end)
{
    if (groups_begin >= groups_end)
        throw std::runtime_error("There is no groups to parse");
}
    

template<
    typename GroupIterator,
    typename = std::enable_if_t<isGroupIterator_v<GroupIterator>>>
ParseTreeAndEnd<GroupIterator>
parseArithmeticExpression(GroupIterator& groups_begin, GroupIterator& groups_end)
{
    std::unique_ptr<ParseTreeNonTerminal<ARITH_EXPR> > parse_tree_ptr;
    ParseTreeAndEnd<GroupIterator> sub_parse_tree;

    GroupIterator current_position = groups_begin;

    if (current_position->token == LEFT_PAREN)
    {
        sub_parse_tree = parseParenthesesEnclosedArithmeticExpression(
            groups_begin, groups_end);
        parse_tree_ptr->addChild(std::move(*sub_parse_tree.parse_tree_ptr));
        current_position = sub_parse_tree.end;
    }
    else if (current_position->token == NUMBER)
    {
        parse_tree_ptr->addChild(ParseTreeTerminal<NUMBER>());
        ++current_position;
    }

    if (current_position == groups_end 
        || !isOperatorBeginning(current_position->token))
    {
        return makeParseTreeAndEnd(
            parse_tree_ptr,
            current_position);
    }

    sub_parse_tree = parseOperator(current_position, groups_end);
    parse_tree_ptr->addChild(*sub_parse_tree.parse_tree_ptr);
    current_position = sub_parse_tree.end;
    
    sub_parse_tree = parseArithmeticExpression(current_position, groups_end);
    parse_tree_ptr->addChild(*sub_parse_tree.parse_tree_ptr);
    current_position = sub_parse_tree.end;

    return makeParseTreeAndEnd(
        parse_tree_ptr,
        current_position);
}


template<
    typename GroupIterator,
    typename = std::enable_if_t<isGroupIterator_v<GroupIterator>>>
ParseTreeAndEnd<GroupIterator>
parseParenthesesEnclosedArithmeticExpression(
    GroupIterator& groups_begin,
    GroupIterator& groups_end)
{
    ParseTreeAndEnd<GroupIterator> sub_parse_tree;
    GroupIterator current_position = groups_begin;

    if (groups_begin->token != LEFT_PAREN)
    {
        throw std::invalid_argument("Parentheses Enclosed Arithmetic"
            "Expression must start with a left parentheses");
    }
    ++current_position;

    sub_parse_tree = parseArithmeticExpression(current_position, groups_end);
    current_position = sub_parse_tree.end;

    if (current_position->token != RIGHT_PAREN)
    {
        throw std::invalid_argument("Missing right parenthese");
    }
    ++current_position;

    return makeParseTreeAndEnd(
        sub_parse_tree.parse_tree_ptr,
        current_position);
}

template<
    typename GroupIterator,
    typename = std::enable_if_t<isGroupIterator_v<GroupIterator>>>
ParseTreeAndEnd<GroupIterator>
parseOperator(GroupIterator& groups_begin, GroupIterator& groups_end)
{
    assertTypeIsGroupIterator(groups_begin);

    auto parsed_tree_ptr = std::unique_ptr<ParseTreeNonTerminal<OPERATOR>>(
        new ParseTreeNonTerminal<OPERATOR>);
    switch(groups_begin->token) {
    case ADD_OP:
        parsed_tree_ptr->addChild(ParseTreeTerminal<ADD_OP>());
        break;
    case SUB_OP:
        parsed_tree_ptr->addChild(ParseTreeTerminal<SUB_OP>());
        break;
    default:
        throw std::invalid_argument("Operator must be a valid operator");
    }
    return makeParseTreeAndEnd(parsed_tree_ptr, ++groups_begin);
        
}

#endif

