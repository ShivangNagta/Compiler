#include "lexer.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cctype>
#include <stack>

using namespace std;

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
    stack<int> parenStack; 

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

        if (ch == '/' && i + 1 < code.size()) {
            if (code[i + 1] == '/') {
                while (i < code.size() && code[i] != '\n') {
                    i++;
                }
                continue;
            } else if (code[i + 1] == '*') {
                i += 2;
                while (i + 1 < code.size() && !(code[i] == '*' && code[i + 1] == '/')) {
                    i++;
                }
                i += 2;
                continue;
            }
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

            if (i < code.size() && isalpha(code[i])) {
                tokens.push_back({"ERROR", "Invalid identifier: cannot start with a digit"});
                return tokens;
            }

            if (number.back() == '.') {
                tokens.push_back({"ERROR", "Invalid identifier: '.' cannot be interpreted"});
            } else {
                tokens.push_back({"NUMBER", number});
            }
            continue;
        }

        // Handle string literals
        if (ch == '"') {
            string strLiteral;
            i++;
            while (i < code.size() && code[i] != '"') {
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

        // Logical and compound operators
        string twoCharOps[] = {"==", "!=", "<=", ">=", "||", "&&", "+=", "-=", "*=", "/=", "%="};
        bool matched = false;
        for (const string& op : twoCharOps) {
            if (i + 1 < code.size() && code.substr(i, 2) == op) {
                tokens.push_back({"OPERATOR", op});
                i += 2;
                matched = true;
                break;
            }
        }
        if (matched) continue;

        // Single-character operators and punctuation
        string singleCharOps = "+-*/%;<>()";
        if (singleCharOps.find(ch) != string::npos) {
            tokens.push_back({"OPERATOR", string(1, ch)});
            i++;
            continue;
        }



        // Handle unknown characters
        tokens.push_back({"ERROR", string("Unknown character: ") + ch});
        i++;
    }

    // Check for unmatched opening parentheses
    if (!parenStack.empty()) {
        tokens.push_back({"ERROR", "Unmatched opening parenthesis '('"});
        return tokens;
    }

    return tokens;
}
