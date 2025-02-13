
#include <iostream>
#include <vector>
#include <string>
#include "lexer.h" 

using namespace std;



int main() {
    const string filename = "Input/input_code_6.txt";
    string code = read_file_contents(filename);

    vector<Token> tokens = get_tok(code);

    cout << "Tokens:\n";
    for (const auto& token : tokens) {
        cout << "Type: " << token.type << " | Value: " << token.value << "\n";
    }
    cout << "\n";

    


    return 0;
}