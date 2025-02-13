#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "lexer.h"

enum NodeType { BinaryOp, Variable, Number, String };

struct Node {
    NodeType type;
    virtual ~Node() = default;
};

struct BinaryExprNode : public Node {
    std::string op;
    Node* l_child;
    Node* r_child;

    BinaryExprNode(const std::string& op, Node* l_child, Node* r_child) 
        : op(op), l_child(l_child), r_child(r_child) { type = BinaryOp; }
};

struct VariableExprNode : public Node {
    std::string name;

    VariableExprNode(const std::string& name) : name(name) { type = Variable; }
};

struct NumberExprNode : public Node {
    double value;

    NumberExprNode(double value) : value(value) { type = Number; }
};

struct StringExprNode : public Node {
    std::string value;

    StringExprNode(const std::string& value) : value(value) { type = String; }
};

extern std::unordered_map<std::string, int> BinopPrecedence;

Node* ParseExpression(std::vector<Token>& tokens, size_t& index);
Node* ParsePrimary(std::vector<Token>& tokens, size_t& index);
Node* ParseBinOpRHS(int exprPrec, Node* lhs, std::vector<Token>& tokens, size_t& index);
