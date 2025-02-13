#pragma once

#include <string>
#include <vector>

typedef struct Token {
    std::string type;
    std::string value;
} Token ;

std::string read_file_contents(const std::string& filename);
std::vector<Token> get_tok(const std::string& code);
bool isIdentifierChar(char ch);

