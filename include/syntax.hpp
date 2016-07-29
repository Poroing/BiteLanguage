#ifndef _H_SYNTAXANALYZER_H_
#define _H_SYNTAXANALYZER_H_

#include "parsetree.hpp"
#include "lexer.hpp"

class SyntaxAnalyzer
{
public:
    SyntaxAnalyzer(std::istream& program_stream);

    void getNextGroup();

    void generateParseTree();
    std::unique_ptr<ParseTreeBase> getParseTree(Token token);

    template<Token tree_token> void putToken(ParseTree<tree_token>& tree,
        Token token)
    {
        tree.childs->emplace_back(getParseTree(token));
        getNextGroup();
    }
        

private:
    void checkCurrentToken(Token wanted_token);

    std::unique_ptr<ParseTree<ARITH_EXPR> > arithExpr();
    std::unique_ptr<ParseTree<LEFT_PAREN> > leftParen();
    std::unique_ptr<ParseTree<RIGHT_PAREN> > leftParen();
    std::unique_ptr<ParseTree<ARITH_OPERATOR> > arithOperator();
    std::unique_ptr<ParseTree<ADD_OP> > addOp();
    std::unique_ptr<ParseTree<SUB_OP> > subOP();
    std::unique_ptr<ParseTree<NUMBER> > number();

    Lexer lexer;
    ParseTree<PROGRAM> program_parse_tree;
    Group current_group;
    

};

#endif
