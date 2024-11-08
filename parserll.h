#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "scanner.h"
#include <stack>


class ParserLL
{
    private:
        Scanner* scanner;
        vector<string> reservedWords;
        map<string, vector<vector<string>>> firstp = {
            {"Program", vector<vector<string>>({{"char", "int", "void", "EOF", "$"}})}, 
            {"Function", vector<vector<string>>({{"char", "int"}, {"void"}})}, 
            {"mFunction", vector<vector<string>>({{"EOF"}, {"char", "int", "void"}})}, 
            {"mVarDeclaration", vector<vector<string>>({{"RBRACKET"}, {"char", "int"}})}, 
            {"commaDeclaration" , vector<vector<string>>({{"SEMICOLON"}, {"COMMA"}})}, 
            {"VarDeclaration", vector<vector<string>>({{"ID"}})}, 
            {"possibleArrDef" , vector<vector<string>>({{"SEMICOLON"}, {"LBRACE"}})}, 
            {"Type" , vector<vector<string>>({{"char"}, {"int"}})}, 
            {"ParamTypes" , vector<vector<string>>({{"void"}, {"char", "int"}})}, 
            {"possibleArr" , vector<vector<string>>({{"RPAREN"}, {"LBRACE"}})}, 
            {"mParamTypes" , vector<vector<string>>({{"RPAREN"}, {"COMMA"}})}, 
            {"Statement" , vector<vector<string>>({{"if"}, {"while"}, {"for"}, {"return"}, {"ID"}, {"LBRACKET"}, {"SEMICOLON"}})}, 
            {"elseStatement" , vector<vector<string>>({{"RBRACKET"}, {"else"}})}, 
            {"mStatement" , vector<vector<string>>({{"RBRACKET"}, {"if", "while", "for", "return", "ID", "LBRACKET", "SEMICOLON"}})}, 
            {"IDdecision" , vector<vector<string>>({{"LBRACE", "RECEIVE"}, {"LPAREN"}})}, 
            {"Assign" , vector<vector<string>>({{"ID"}})}, 
            {"possibleIDCall" , vector<vector<string>>({{"RECEIVE"}, {"LBRACE"}})}, 
            {"possibleAssign" , vector<vector<string>>({{"SEMICOLON", "RPAREN"}, {"ID"}})},
            {"Expression" , vector<vector<string>>({{"MINUS"}, {"NOT"}, {"ID"}, {"LPAREN"}, {"integerconstant"}, {"charconstant"}, {"stringconstant"}})}, 
            {"Operation" , vector<vector<string>>({{"SUM", "MINUS", "MULT", "DIV"}, {"EQ", "NEQ", "LEQ", "LT", "GEQ", "GT"}, {"AND", "OR"}, {"RPAREN", "SEMICOLON", "RBRACE"}})},
            {"commaExpression" , vector<vector<string>>({{"RPAREN", "RBRACE"}, {"COMMA"}})},
            {"possibleExpression_comma" , vector<vector<string>>({{"RPAREN", "RBRACE"}, {"MINUS", "NOT", "ID", "LPAREN", "integerconstant", "charconstant", "stringconstant"}})},
            {"IDCall" , vector<vector<string>>({{"RPAREN", "SEMICOLON", "RBRACE"}, {"LBRACE"}})}, 
            {"possibleExpression" , vector<vector<string>>({{"SEMICOLON"}, {"MINUS", "NOT", "ID", "LPAREN", "integerconstant", "charconstant", "stringconstant"}})}, 
            {"BinOP" , vector<vector<string>>({{"SUM"}, {"MINUS"}, {"MULT"}, {"DIV"}})}, 
            {"RelOP" , vector<vector<string>>({{"EQ"}, {"NEQ"}, {"LEQ"}, {"LT"}, {"GEQ"}, {"GT"}})}, 
            {"LogOP", vector<vector<string>>({ {"AND"}, {"OR"} })}
        };
        Token getTokenFromScanner();
        bool is_reservedword(Token &n);

    public:
        ParserLL();
        bool process(string);
};
