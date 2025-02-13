#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>

struct Token {
    std::string type;
    std::string value;
};

std::string read_file_contents(const std::string& filename);
std::vector<Token> get_tok(const std::string& code);
bool isIdentifierChar(char ch);

#endif
