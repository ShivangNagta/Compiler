#include <iostream>
#include <vector>
#include "lexer.h"
#include "parser_old.h"

using namespace std;

int main() {
    const string filename = "Input/input_code_5.txt";
    string code = read_file_contents(filename);

    // Tokenize the input
    vector<Token> tokens = get_tok(code);

    // Print tokens
    cout << "Tokens:\n";
    for (const auto& token : tokens) {
        cout << "Type: " << token.type << " | Value: " << token.value << "\n";
    }
    cout << "\n";

    // Set the global token list for the parser
    Tokens = tokens;
    CurrentTokenIdx = 0;

    // Parse the input expression
    ExprAST* ast = ParseExpression();
    if (ast) {
        cout << "Parse Tree:\n";
        ast->print();
        delete ast; // Prevent memory leak
    } else {
        cerr << "Parsing failed.\n";
    }

    return 0;
}



