
#include <iostream>
#include <vector>
#include <string>
#include "lexer.h" 

using namespace std;


struct ASTNode {
    virtual ~ASTNode() = default;
    virtual void Print(int indent = 0) const = 0; 
};

struct VariableExpr : public ASTNode {
    string name;
    VariableExpr(const string& name) : name(name) {}

    void Print(int indent) const override {
        cout << string(indent, ' ') << "Variable: " << name << endl;
    }
};

struct BinaryExpr : public ASTNode {
    char op;
    ASTNode* lhs;
    ASTNode* rhs;
    BinaryExpr(char op, ASTNode* lhs, ASTNode* rhs)
        : op(op), lhs(lhs), rhs(rhs) {}

    ~BinaryExpr() {
        delete lhs;
        delete rhs;
    }

    void Print(int indent) const override {
        cout << string(indent, ' ') << "Binary Operator: " << op << endl;
        lhs->Print(indent + 2);
        rhs->Print(indent + 2);
    }
};


class Parser {
public:
    Parser(const vector<Token>& tokens) : tokens(tokens), pos(0) {}

    ASTNode* ParseExpression() {
        ASTNode* lhs = ParsePrimary();
        return ParseBinOpRHS(0, lhs);
    }

    ~Parser() {
    }

private:
    vector<Token> tokens;
    size_t pos;

    ASTNode* ParsePrimary() {
        if (pos >= tokens.size()) {
            throw runtime_error("Unexpected end of input");
        }

        Token token = tokens[pos++];
        if (token.type == "IDENTIFIER") {
            return new VariableExpr(token.value);
        } else {
            throw runtime_error("Unexpected token: " + token.value);
        }
    }

    ASTNode* ParseBinOpRHS(int precedence, ASTNode* lhs) {
        while (true) {
            if (pos >= tokens.size()) {
                return lhs;
            }

            Token token = tokens[pos];
            if (token.type != "OPERATOR") {
                return lhs;
            }

            int tokenPrecedence = GetOperatorPrecedence(token.value[0]);
            if (tokenPrecedence < precedence) {
                return lhs;
            }

            char op = token.value[0];
            pos++;

            ASTNode* rhs = ParsePrimary();
            if (pos < tokens.size()) {
                Token nextToken = tokens[pos];
                if (nextToken.type == "OPERATOR") {
                    int nextPrecedence = GetOperatorPrecedence(nextToken.value[0]);
                    if (tokenPrecedence < nextPrecedence) {
                        rhs = ParseBinOpRHS(tokenPrecedence + 1, rhs);
                    }
                }
            }

            lhs = new BinaryExpr(op, lhs, rhs);
        }
    }

    int GetOperatorPrecedence(char op) {
        switch (op) {
            case '<': return 5;
            case '>': return 5;
            case '>=': return 5;
            case '<=': return 5;
            case '+': return 10;
            case '-': return 10;
            case '%': return 20;
            case '*': return 20;
            case '/': return 20;
            default: return -1;
        }
    }
};


void PrintAST(const ASTNode* node, int indent = 0) {
    node->Print(indent);
}

int main() {
    const string filename = "Input/input_code_6.txt";
    string code = read_file_contents(filename);

    vector<Token> tokens = get_tok(code);

    cout << "Tokens:\n";
    for (const auto& token : tokens) {
        cout << "Type: " << token.type << " | Value: " << token.value << "\n";
    }
    cout << "\n";

    Parser parser(tokens);
    ASTNode* ast = parser.ParseExpression();

    cout << "Abstract Syntax Tree (AST):" << endl;
    PrintAST(ast);

    delete ast;

    return 0;
}