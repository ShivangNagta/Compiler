#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <cctype>

using namespace std;

enum TokenType {
    DEF,
    EXTERN,
    IDENTIFIER,
    NUMBER,
    OPERATOR,
    COMMENT,
    STRING,
    LPAREN,
    RPAREN,
    ERROR
};

struct Token {
    TokenType type;
    string value;
};

bool isIdentifierChar(char ch);

vector<Token> get_tok(const string& code);


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

int main() {
    const string filename = "input_code.txt";
    string code = read_file_contents(filename);

    vector<Token> tokens = get_tok(code);

    for (const auto& token : tokens) {
        cout << "Type: " << token.type << ", Value: " << token.value << endl;
    }

    return 0;
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
            tokens.push_back({COMMENT, comment});
            continue;
        }

        // Handle identifiers and keywords
        if (isalpha(ch)) {
            string identifier;
            while (i < code.size() && isIdentifierChar(code[i])) {
                identifier += code[i];
                i++;
            }
            if (identifier == "def") {
                tokens.push_back({DEF, identifier});
            } else if (identifier == "extern") {
                tokens.push_back({EXTERN, identifier});
            } else {
                tokens.push_back({IDENTIFIER, identifier});
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
                        tokens.push_back({ERROR, "Invalid number format: multiple decimal points"});
                        return tokens;
                    }
                    hasDecimal = true;
                }
                number += code[i];
                i++;
            }
            tokens.push_back({NUMBER, number});
            continue;
        }

        // Handle string literals
        if (ch == '"') {
            string strLiteral;
            i++;
            while (i < code.size() && code[i] != '"') {
                strLiteral += code[i];
                i++;
            }
            if (i >= code.size()) {
                tokens.push_back({ERROR, "Unclosed string literal"});
                return tokens;
            }
            i++;
            tokens.push_back({STRING, strLiteral});
            continue;
        }

        // Handle operators
        if (ch == '+' || ch == '-' || ch == '<' || ch == '>' || ch == '=') {
            tokens.push_back({OPERATOR, string(1, ch)});
            i++;
            continue;
        }

        // Handle parentheses
        if (ch == '(') {
            tokens.push_back({LPAREN, "("});
            i++;
            continue;
        }

        if (ch == ')') {
            tokens.push_back({RPAREN, ")"});
            i++;
            continue;
        }

        // Handle unknown characters
        tokens.push_back({ERROR, string("Unknown character: ") + ch});
        i++;
    }

    return tokens;
}
