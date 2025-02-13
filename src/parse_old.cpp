#include "parser_old.h"
#include <cctype>

std::vector<Token> Tokens;  
size_t CurrentTokenIdx = 0; 


static Token getNextToken() {
    if (CurrentTokenIdx < Tokens.size())
        return Tokens[CurrentTokenIdx++];
    return {"EOF", ""};
}

// Operator precedence map
map<char, int> BinopPrecedence = {
    {'+', 10}, {'-', 10},
    {'*', 20}, {'/', 20},
    {'%', 20},
    {'<', 5},  {'>', 5},  {'=', 5} 
};

// Parses numbers and variables
ExprAST* ParsePrimary() {
    Token tok = getNextToken();

    if (tok.type == "NUMBER") {
        return new NumberExprAST(stod(tok.value));
    } 
    else if (tok.type == "IDENTIFIER") {
        return new VariableExprAST(tok.value);
    }
    
    cerr << "Error: Unexpected token in primary expression: " << tok.value << endl;
    return nullptr;
}

// Parses binary expressions with precedence handling
ExprAST* ParseBinOpRHS(int ExprPrec, ExprAST* LHS) {
    while (true) {
        Token tok = getNextToken();
        if (tok.type != "OPERATOR") return LHS;

        char op = tok.value[0];
        int TokPrec = BinopPrecedence[op];

        if (TokPrec < ExprPrec) return LHS;

        ExprAST* RHS = ParsePrimary();
        if (!RHS) return nullptr;

        Token nextTok = getNextToken();
        int NextPrec = BinopPrecedence[nextTok.value[0]];

        if (TokPrec < NextPrec) {
            RHS = ParseBinOpRHS(TokPrec + 1, RHS);
            if (!RHS) return nullptr;
        }

        LHS = new BinaryExprAST(op, LHS, RHS);
    }
}

// Parses full expressions
ExprAST* ParseExpression() {
    ExprAST* LHS = ParsePrimary();
    if (!LHS) return nullptr;

    return ParseBinOpRHS(0, LHS);
}
