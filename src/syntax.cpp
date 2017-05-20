#include "syntax.hpp"
#include "number.hpp"
#include "tokens.hpp"

SyntaxAnalyzer::SyntaxAnalyzer(std::istream& program_stream):
    lexer(program_stream)
{};

void SyntaxAnalyzer::generateParseTree()
{
    
}

std::unique_ptr<ParseTreeBase> getParseTree(Token token)
{
    switch (token) {
    case ARITH_EXPR:
        return arithExpr();
    case NUMBER:
        return number();
    case LEFT_PAREN:
        return leftParen();
    case RIGHT_PAREN:
        return rightParen();
    case ARITH_OPERATOR:
        return arithOperator();
    case ADD_OP:
        return addOperator();
    case SUB_OP:
        return subOperator();
    default:
        return std::unique_ptr<ParseTreeBase>();
    }
}

std::unique_ptr<ParseTree<ARITH_EXPR> > arithExpr()
{
    auto arith_expr_ptr = make_unique<ParseTree<ARITH_EXPR> >()
    switch (current_group.token) {
    case NUMBER:
        putToken(*arith_expr_ptr, NUMBER);
        break;
    case LEFT_PAREN:
        putToken(*arith_expr_ptr, LEFT_PAREN);
        putToken(*arith_expr_ptr, ARITH_EXPR);
        putToken(*arith_expr_ptr, RIGHT_PAREN);
        break;
    default:
        putToken(*arith_expr_ptr, ARITH_EXPR);
        putToken(*arith_expr_ptr, ARITH_OPERATOR);
        putToken(*arith_expr_ptr, ARITH_EXPR);
    }
    return arith_expr_ptr;
}

std::unique_ptr<ParseTree<LEFT_PAREN> > leftParen()
{
    if (current_group.token != LEFT_PAREN) throw std::runtime_error("ERROR")
    return make_unique<ParseTree<LEFT_PAREN> >();
}

std::unique_ptr<ParseTree<RIGHT_PAREN> > rightParen()
{
    if (current_group.token != RIGHT_PAREN) throw std::runtime_error("ERROR")
    return make_unique<ParseTree<RIGHT_PAREN> >();
}

std::unique_ptr<ParseTree<ARITH_EXPR> > arithOperator()
{
    auto 
    

std::unique_ptr<ParseTree<NUMBER> > number()
{
    //TODO: Write a good error
    if (current_group.token != NUMBER) throw std::runtime_error("ERROR")
    std::string number_string = current_group.lexeme;
    auto number_ptr = make_unique<ParseTree<NUMBER> >()
    number_ptr->value = braxNumberToInteger(number_string);
    return number_ptr;
}
        
