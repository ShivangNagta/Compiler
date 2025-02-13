// #pragma once

// #include <string>
// #include <vector>
// #include <unordered_map>

// #include "lexer.h"

// typedef struct Node {
//     union {
//         struct BinaryExpression{
//             Node* lhs;
//             Node* rhs;
//         };
//         struct VariableExpression{
//             std::string variable_name;
//         };
//     };
// } Node;

// std::unordered_map< std::string, int> BinopPrecedence = {
//     {"+", 1},
//     {"-", 1},
//     {"*", 2},
//     {"/", 2}
// };


// void ParseExpression(std::vector<Token> &tokens);
// void ParsePrimary();
// void ParseBinOpRHS();