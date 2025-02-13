#include "parser_new.h"
#include <iostream>

Node* ParsePrimary(std::vector<Token>& tokens, size_t& index) {
    if (index >= tokens.size()) return nullptr;

    Token token = tokens[index++];
    
    if (token.type == "IDENTIFIER") {
        return new VariableExprNode(token.value);
    } 
    else if (token.type == "NUMBER") {
        return new NumberExprNode(std::stod(token.value));
    } 
    else if (token.type == "STRING") {
        return new StringExprNode(token.value);
    } 
    else {
        std::cerr << "Unexpected token: " << token.value << std::endl;
        return nullptr;
    }
}

int GetTokenPrecedence(const std::string& op) {
    if (BinopPrecedence.find(op) != BinopPrecedence.end()) 
        return BinopPrecedence[op];
    return -1;
}

Node* ParseBinOpRHS(int exprPrec, Node* lhs, std::vector<Token>& tokens, size_t& index) {
    while (true) {
        if (index >= tokens.size()) return lhs;

        std::string op = tokens[index].value;
        int tokenPrec = GetTokenPrecedence(op);

        if (tokenPrec < exprPrec) return lhs;

        index++;
        Node* rhs = ParsePrimary(tokens, index);
        if (!rhs) return nullptr;

        int nextPrec = GetTokenPrecedence(tokens[index].value);
        if (tokenPrec < nextPrec) {
            rhs = ParseBinOpRHS(tokenPrec + 1, rhs, tokens, index);
            if (!rhs) return nullptr;
        }

        lhs = new BinaryExprNode(op, lhs, rhs);
    }
}

std::unordered_map<std::string, int> BinopPrecedence = {
    {"+", 1}, {"-", 1}, {"*", 2}, {"/", 2}, {"%", 2}, 
    {"<", 0}, {">", 0}, {"<=", 0}, {">=", 0}, {"&&", 3}
};

Node* ParseExpression(std::vector<Token>& tokens, size_t& index) {
    Node* lhs = ParsePrimary(tokens, index);
    if (!lhs) return nullptr;

    return ParseBinOpRHS(0, lhs, tokens, index);
}
