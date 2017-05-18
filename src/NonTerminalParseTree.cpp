#include "parsetree.hpp"

bool ParseTreeBase::isTerminal()
{
    return ::isTerminal(this->token());
};

Token NonTerminalParseTree::getChildToken(int place)
{
    checkPlace(place);
    return getChildTokenNoCheck(place);
};
    
NonTerminalParseTree::NonTerminalParseTree()
{
    chids.resize(this->numberOfChilds());
};

void NonTerminalParseTree::setChild(int place,
    std::unique_ptr<ParseTreeBase> child)
{
    checkIfChildIsAtTheRightPlace(place, child);
    childs[place] = child;
};

void NonTerminalParseTree::checkIfChildIsAtTheRightPlace(int place,
    std::unique_ptr<ParseTreeBase> child)
{
    if (child->token() != this->getChildToken(place)) {
        throw std::runtime_error("Wrong Child Type");
    }
};

void NonTerminalParseTree::checkPlace(int place)
{
    if (place < 0 || place >= this->numberOfChilds()) {
        throw std::runtime_error("Place out of bound");
    }
};
