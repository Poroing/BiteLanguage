#ifndef _H_BRAXGROUP_H_
#define _H_BRAXGROUP_H_

#include "type.hpp"
#include "tokens.hpp"

struct Group {
    Group(const std::string& lexeme, Token token);

    std::string lexeme;
    Token token;
};


template<typename T, typename = void>
struct isGroupIterator : std::false_type {};

template<typename T>
struct isGroupIterator<T, std::enable_if_t<isIterator_v<T>>>
    : std::is_same<getIteratorTraitsValueType_t<T>, Group>
{};

template<typename T>
constexpr bool isGroupIterator_v = isGroupIterator<T>::value;

#endif
