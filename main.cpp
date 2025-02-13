#include <iostream>
#include <vector>
#include "include/lexer.h"

using namespace std;

int main() {
    const string filename = "Input/input_code_5.txt";
    string code = read_file_contents(filename);

    vector<Token> tokens = get_tok(code);

    for (const auto& token : tokens) {
        cout << "Type: " << token.type << "\n" << "Value: " << token.value << "\n\n";
    }

    return 0;
}
