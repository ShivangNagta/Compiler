// #ifndef PARSER_H
// #define PARSER_H

// #include <iostream>
// #include <map>
// #include <vector>
// #include "lexer.h"

// using namespace std;

// extern std::vector<Token> Tokens; 
// extern size_t CurrentTokenIdx; 

// // Base class for AST nodes
// class ExprAST {
// public:
//     virtual ~ExprAST() = default;
//     virtual void print() const = 0;
// };

// // AST node for numeric literals
// class NumberExprAST : public ExprAST {
//     double Val;
// public:
//     NumberExprAST(double val) : Val(val) {}
//     void print() const override { cout << "Number: " << Val << endl; }
// };

// // AST node for variable references
// class VariableExprAST : public ExprAST {
//     string Name;
// public:
//     VariableExprAST(const string &name) : Name(name) {}
//     void print() const override { cout << "Variable: " << Name << endl; }
// };

// // AST node for binary operations
// class BinaryExprAST : public ExprAST {
//     char Op;
//     ExprAST *LHS, *RHS;
// public:
//     BinaryExprAST(char op, ExprAST* lhs, ExprAST* rhs)
//         : Op(op), LHS(lhs), RHS(rhs) {}

//     ~BinaryExprAST() {
//         delete LHS;
//         delete RHS;
//     }

//     void print() const override {
//         cout << "Binary Op: " << Op << endl;
//         cout << "LHS -> ";
//         LHS->print();
//         cout << "RHS -> ";
//         RHS->print();
//     }
// };

// // Precedence map for operators
// extern map<char, int> BinopPrecedence;

// // Function prototypes
// ExprAST* ParseExpression();
// ExprAST* ParsePrimary();
// ExprAST* ParseBinOpRHS(int ExprPrec, ExprAST* LHS);

// #endif
