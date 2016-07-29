#ifndef _H_BITEPARSETREE_H_
#define _H_BITEPARSETREE_H_

#include <type_traits>
#include <list>
#include <memory>

template<Token token>
struct is_terminal : std::false_type {};

template<>
struct is_terminal<NUMBER> : std::true_type {};

template<>
struct is_terminal<ADD_OP> : std::true_type {};

template<>
struct is_terminal<SUB_OP> : std::true_type {};

template<>
struct is_terminal<LEFT_PAREN> : std::true_type {};

template<>
struct is_terminal<RIGHT_PAREN> : std::true_type {};


struct ParseTreeBase
{
    static virtual Token token() = 0;
};

template<Token token>
struct ParseTree : public ParseTreeBase
{
    static virtual Token token() { return token; };
    
    std::enable_if<
            is_terminal<token>::value,
            std::list<std::unique_ptr<ParseTree> > > 
        childs;
};

template<>
struct ParseTree<NUMBER, false> : public ParseTreeBase
{
    static virtual Token token() { return NUMBER; };
    int value;
};

#endif
