#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <regex>

using namespace std;

// Token types
enum TokenType {
    TOK_DEF,
    TOK_EXTERN,
    TOK_IDENTIFIER,
    TOK_NUMBER,
    TOK_OPERATOR,
    TOK_COMMENT,
    TOK_STRING,
    TOK_LPAREN,
    TOK_RPAREN,
    TOK_INVALID_NUMBER,
    TOK_INVALID_STRING,
    TOK_UNKNOWN
};

// Token structure
struct Token {
    TokenType type;
    string value;
};

// Tokenizer function
vector<Token> tokenize(const string& code) {
    vector<Token> tokens;
    regex token_patterns[] = {
        regex(R"(def)"),                     // TOK_DEF
        regex(R"(extern)"),                  // TOK_EXTERN
        regex(R"([a-zA-Z_][a-zA-Z0-9_]*)"),  // TOK_IDENTIFIER
        regex(R"(\d+(\.\d+)?)"),             // TOK_NUMBER
        regex(R"([\+\-\*/<>=!]+)"),          // TOK_OPERATOR
        regex(R"(#.*)"),                     // TOK_COMMENT
        regex(R"("[^"]*")"),                 // TOK_STRING
        regex(R"(\()"),                      // TOK_LPAREN
        regex(R"(\))"),                      // TOK_RPAREN
        regex(R"(\s+)")                      // Whitespace (ignored)
    };

    smatch match;
    string remaining_code = code;

    while (!remaining_code.empty()) {
        bool matched = false;

        // Try to match each token pattern
        for (size_t i = 0; i < sizeof(token_patterns) / sizeof(token_patterns[0]); ++i) {
            if (regex_match(remaining_code, match, token_patterns[i], regex_constants::match_continuous)) {
                string value = match.str(0);
                Token token;

                // Determine token type
                switch (i) {
                    case 0: token.type = TOK_DEF; break;
                    case 1: token.type = TOK_EXTERN; break;
                    case 2: token.type = TOK_IDENTIFIER; break;
                    case 3:
                        if (count(value.begin(), value.end(), '.') > 1) {
                            token.type = TOK_INVALID_NUMBER;
                        } else {
                            token.type = TOK_NUMBER;
                        }
                        break;
                    case 4: token.type = TOK_OPERATOR; break;
                    case 5: token.type = TOK_COMMENT; break;
                    case 6:
                        if (value.size() < 2 || value.front() != '"' || value.back() != '"') {
                            token.type = TOK_INVALID_STRING;
                        } else {
                            token.type = TOK_STRING;
                            value = value.substr(1, value.size() - 2); // Remove quotes
                        }
                        break;
                    case 7: token.type = TOK_LPAREN; break;
                    case 8: token.type = TOK_RPAREN; break;
                    case 9: break; // Ignore whitespace
                    default: token.type = TOK_UNKNOWN; break;
                }

                token.value = value;
                if (token.type != TOK_UNKNOWN && i != 9) { // Skip whitespace and unknown tokens
                    tokens.push_back(token);
                }
                remaining_code = match.suffix().str();
                matched = true;
                break;
            }
        }

        // Handle unmatched characters
        if (!matched) {
            Token token;
            token.type = TOK_UNKNOWN;
            token.value = remaining_code.substr(0, 1);
            tokens.push_back(token);
            remaining_code = remaining_code.substr(1);
        }
    }

    return tokens;
}

// Function to print tokens
void print_tokens(const vector<Token>& tokens) {
    for (const auto& token : tokens) {
        cout << "Type: ";
        switch (token.type) {
            case TOK_DEF: cout << "DEF"; break;
            case TOK_EXTERN: cout << "EXTERN"; break;
            case TOK_IDENTIFIER: cout << "IDENTIFIER"; break;
            case TOK_NUMBER: cout << "NUMBER"; break;
            case TOK_OPERATOR: cout << "OPERATOR"; break;
            case TOK_COMMENT: cout << "COMMENT"; break;
            case TOK_STRING: cout << "STRING"; break;
            case TOK_LPAREN: cout << "LPAREN"; break;
            case TOK_RPAREN: cout << "RPAREN"; break;
            case TOK_INVALID_NUMBER: cout << "INVALID_NUMBER"; break;
            case TOK_INVALID_STRING: cout << "INVALID_STRING"; break;
            case TOK_UNKNOWN: cout << "UNKNOWN"; break;
        }
        cout << ", Value: " << token.value << endl;
    }
}

int main() {
    string code = R"(
        def fib(x)
        if x < 3 then
            1
        else
            fib(x-1) + fib(x-2)
    )";

    vector<Token> tokens = tokenize(code);
    print_tokens(tokens);

    return 0;
}