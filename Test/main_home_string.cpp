#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cctype>

using namespace std;

struct Token {
    string type;  
    string value;
};


bool isIdentifierChar(char ch);
vector<Token> get_tok(const string& code);
string read_file_contents(const string& filename);



int main() {
    const string filename = "input_code_1.txt";
    string code = read_file_contents(filename);

    vector<Token> tokens = get_tok(code);

    for (const auto& token : tokens) {
        cout << "Type: " << token.type << "\n" <<  "Value: " << token.value << "\n\n";
    }

    return 0;
}



string read_file_contents(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << "\n";
        return "";
    }

    stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}



bool isIdentifierChar(char ch) {
    return isalnum(ch) || ch == '_';
}

vector<Token> get_tok(const string& code) {
    vector<Token> tokens;
    int i = 0;
    while (i < code.size()) {
        char ch = code[i];

        // Skip whitespace
        if (isspace(ch)) {
            i++;
            continue;
        }

        // Handle comments
        if (ch == '#') {
            string comment;
            while (i < code.size() && code[i] != '\n') {
                comment += code[i];
                i++;
            }
            tokens.push_back({"COMMENT", comment});
            continue;
        }

        // Handle identifiers and keywords
        if (isalpha(ch) || ch == '_') {
            string identifier;
            while (i < code.size() && isIdentifierChar(code[i])) {
                identifier += code[i];
                i++;
            }
            if (identifier == "def") {
                tokens.push_back({"DEF", identifier});
            } else if (identifier == "extern") {
                tokens.push_back({"EXTERN", identifier});
            } else if (identifier == "if") {
                tokens.push_back({"IF", identifier});
            } else if (identifier == "else") {
                tokens.push_back({"ELSE", identifier});
            } else if (identifier == "then") {
                tokens.push_back({"THEN", identifier});
            } else if (identifier == "print") {
                tokens.push_back({"PRINT", identifier});
            } else {
                tokens.push_back({"IDENTIFIER", identifier});
            }
            continue;
        }

        // Handle numbers
        if (isdigit(ch) || ch == '.') {
            string number;
            bool hasDecimal = false;
            while (i < code.size() && (isdigit(code[i]) || code[i] == '.')) {
                if (code[i] == '.') {
                    if (hasDecimal) {
                        tokens.push_back({"ERROR", "Invalid number format: multiple decimal points"});
                        return tokens;
                    }
                    hasDecimal = true;
                }
                number += code[i];
                i++;
            }

            // Check if the number is followed by an alphabetic character
            if (i < code.size() && isalpha(code[i])) {
                tokens.push_back({"ERROR", "Invalid identifier: cannot start with a digit"});
                return tokens;
            }

            tokens.push_back({"NUMBER", number});
            continue;
        }

        // Handle string literals
        if (ch == '"') {
            string strLiteral;
            i++;
            while (i < code.size() && code[i] != '"') {
                // Handle escape sequences (e.g., \", \\, \n)
                if (code[i] == '\\') {
                    i++;
                    if (i >= code.size()) {
                        tokens.push_back({"ERROR", "Unclosed string literal"});
                        return tokens;
                    }
                    switch (code[i]) {
                        case 'n': strLiteral += '\n'; break;
                        case 't': strLiteral += '\t'; break;
                        case '\\': strLiteral += '\\'; break;
                        case '"': strLiteral += '"'; break;
                        default:
                            tokens.push_back({"ERROR", "Invalid escape sequence"});
                            return tokens;
                    }
                } else {
                    strLiteral += code[i];
                }
                i++;
            }
            if (i >= code.size()) {
                tokens.push_back({"ERROR", "Unclosed string literal"});
                return tokens;
            }
            i++;
            tokens.push_back({"STRING", strLiteral});
            continue;
        }


        // Logical Operators
        if (ch == '=' && i + 1 < code.size() && code[i + 1] == '=') {
            tokens.push_back({"EQUALITY", "=="});
            i += 2;
            continue;
        }
        if (ch == '!' && i + 1 < code.size() && code[i + 1] == '=') {
            tokens.push_back({"INEQUALITY", "!="});
            i += 2;
            continue;
        }
        if (ch == '|' && i + 1 < code.size() && code[i + 1] == '|') {
            tokens.push_back({"LOGICAL_OR", "||"});
            i += 2;
            continue;
        }
        if (ch == '&' && i + 1 < code.size() && code[i + 1] == '&') {
            tokens.push_back({"LOGICAL_AND", "&&"});
            i += 2;
            continue;
        }
        if (ch == '!') {
            tokens.push_back({"LOGICAL_NOT", "!"});
            i++;
            continue;
        }

        // Handle operators
        if (ch == '+' || ch == '-' || ch == '<' || ch == '>' || ch == '=') {
            tokens.push_back({"OPERATOR", string(1, ch)});
            i++;
            continue;
        }

        if (ch == '!') {
            tokens.push_back({"LOGICAL_NOT", "!"});
            i++;
            continue;
        }

        // Handle parentheses
        if (ch == '(') {
            tokens.push_back({"LPAREN", "("});
            i++;
            continue;
        }

        if (ch == ')') {
            tokens.push_back({"RPAREN", ")"});
            i++;
            continue;
        }
        

        // Handle unknown characters
        tokens.push_back({"ERROR", string("Unknown character: ") + ch});
        i++;


    }

    return tokens;
}