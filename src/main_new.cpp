#include <iostream>
#include "lexer.h"
#include "parser_new.h"

using namespace std;

void PrintAST(Node* node, int depth = 0);


int main() {
    const string filename = "Input/input_code_8.txt";
    string code = read_file_contents(filename);
    std::vector<Token> tokens = get_tok(code);

    cout << "Tokens:\n";
    for (const auto& token : tokens) {
        cout << "Type: " << token.type << " | Value: " << token.value << "\n";
    }
    cout << "\n";

    size_t index = 0;
    Node* ast_node = ParseExpression(tokens, index);

    std::cout << "Parse Tree:\n";
    PrintAST(ast_node);

    return 0;
}



void PrintAST(Node* node, int depth = 0) {
    if (!node) return;
    for (int i = 0; i < depth; i++) std::cout << "  ";

    if (node->type == BinaryOp) {
        auto* binOpNode = static_cast<BinaryExprNode*>(node);
        std::cout << "BinaryOp (" << binOpNode->op << ")\n";
        PrintAST(binOpNode->l_child, depth + 1);
        PrintAST(binOpNode->r_child, depth + 1);
    } 
    else if (node->type == Variable) {
        auto* varNode = static_cast<VariableExprNode*>(node);
        std::cout << "Variable (" << varNode->name << ")\n";
    } 
    else if (node->type == Number) {
        auto* numNode = static_cast<NumberExprNode*>(node);
        std::cout << "Number (" << numNode->value << ")\n";
    } 
    else if (node->type == String) {
        auto* strNode = static_cast<StringExprNode*>(node);
        std::cout << "String (\"" << strNode->value << "\")\n";
    }
}

