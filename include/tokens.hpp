#ifndef _H_TOKENS_H_
#define _H_TOKENS_H_

#include <unordered_map>

enum Token {
    OPERATOR,
    WORD,
    RIGHT_PAREN,
    LEFT_PAREN,
    NUMBER_START,
    NUMBER,
    ADD_OP,
    SUB_OP,
    END_OF_STREAM,
    ARITH_EXPR,
    ARITH_OPERATOR,
    UNKNOWN
};

#endif
