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
        map<Token, vector<vector<Token>>> firstp = {
            {Token(ID, STATE, "Program"), vector<vector<Token>>({{Token(ID, RESERVED, "char"), Token(ID, RESERVED, "int"), Token(ID, RESERVED, "void"), Token(END_OF_FILE), Token(ID, RESERVED, "$")}})}, 
            {Token(ID, STATE, "Function"), vector<vector<Token>>({{Token(ID, RESERVED, "char"), Token(ID, RESERVED, "int")}, {Token(ID, RESERVED, "void")}})}, 
            {Token(ID, STATE, "mFunction"), vector<vector<Token>>({{Token(END_OF_FILE)}, {Token(ID, RESERVED, "char"), Token(ID, RESERVED, "int"), Token(ID, RESERVED, "void")}})}, 
            {Token(ID, STATE, "mVarDeclaration"), vector<vector<Token>>({{Token(RBRACKET)}, {Token(ID, RESERVED, "char"), Token(ID, RESERVED, "int")}})}, 
            {Token(ID, STATE, "commaDeclaration") , vector<vector<Token>>({{Token(SEMICOLON)}, {Token(COMMA)}})}, 
            {Token(ID, STATE, "VarDeclaration"), vector<vector<Token>>({{Token(ID, FREE)}})}, 
            {Token(ID, STATE, "possibleArrDef") , vector<vector<Token>>({{Token(SEMICOLON)}, {Token(LBRACE)}})}, 
            {Token(ID, STATE, "Type") , vector<vector<Token>>({{Token(ID, RESERVED, "char")}, {Token(ID, RESERVED, "int")}})}, 
            {Token(ID, STATE, "ParamTypes") , vector<vector<Token>>({{Token(ID, RESERVED, "void")}, {Token(ID, RESERVED, "char"), Token(ID, RESERVED, "int")}})}, 
            {Token(ID, STATE, "possibleArr") , vector<vector<Token>>({{Token(RPAREN)}, {Token(LBRACE)}})}, 
            {Token(ID, STATE, "mParamTypes") , vector<vector<Token>>({{Token(RPAREN)}, {Token(COMMA)}})}, 
            {Token(ID, STATE, "Statement") , vector<vector<Token>>({{Token(ID, RESERVED, "if")}, {Token(ID, RESERVED, "while")}, {Token(ID, RESERVED, "for")}, {Token(ID, RESERVED, "return")}, {Token(ID, FREE)}, {Token(LBRACKET)}, {Token(SEMICOLON)}})}, 
            {Token(ID, STATE, "elseStatement") , vector<vector<Token>>({{Token(RBRACKET)}, {Token(ID, RESERVED, "else")}})}, 
            {Token(ID, STATE, "mStatement") , vector<vector<Token>>({{Token(RBRACKET)}, {Token(ID, RESERVED, "if"), Token(ID, RESERVED, "while"), Token(ID, RESERVED, "for"), Token(ID, RESERVED, "return"), Token(ID, FREE), Token(LBRACKET), Token(SEMICOLON)}})}, 
            {Token(ID, STATE, "IDdecision") , vector<vector<Token>>({{Token(LBRACE), Token(ASSIGN)}, {Token(LPAREN)}})}, 
            {Token(ID, STATE, "Assign") , vector<vector<Token>>({{Token(ID, FREE)}})}, 
            {Token(ID, STATE, "possibleIDCall") , vector<vector<Token>>({{Token(ASSIGN)}, {Token(LBRACE)}})}, 
            {Token(ID, STATE, "possibleAssign") , vector<vector<Token>>({{Token(SEMICOLON), Token(RPAREN)}, {Token(ID, FREE)}})},
            {Token(ID, STATE, "Expression") , vector<vector<Token>>({{Token(MINUS)}, {Token(RELOP, NT)}, {Token(ID, FREE)}, {Token(LPAREN)}, {Token(NUMBER)}, {Token(LITERAL, CHAR)}, {Token(LITERAL, STRING)}})}, 
            {Token(ID, STATE, "Operation") , vector<vector<Token>>({{Token(PLUS), Token(MINUS), Token(MULT), Token(DIV)}, {Token(RELOP, EQ), Token(RELOP, NE), Token(RELOP, LE), Token(RELOP, LT), Token(RELOP, GE), Token(RELOP, GT)}, {Token(RELOP, AND), Token(RELOP, OR)}, {Token(RPAREN), Token(SEMICOLON), Token(RBRACE)}})},
            {Token(ID, STATE, "commaExpression") , vector<vector<Token>>({{Token(RPAREN), Token(RBRACE)}, {Token(COMMA)}})},
            {Token(ID, STATE, "possibleExpression_comma") , vector<vector<Token>>({{Token(RPAREN), Token(RBRACE)}, {Token(MINUS), Token(RELOP, NT), Token(ID, FREE), Token(LPAREN), Token(NUMBER), Token(LITERAL, CHAR), Token(LITERAL, STRING)}})},
            {Token(ID, STATE, "IDCall") , vector<vector<Token>>({{Token(RPAREN), Token(SEMICOLON), Token(RBRACE)}, {Token(LBRACE)}})}, 
            {Token(ID, STATE, "possibleExpression") , vector<vector<Token>>({{Token(SEMICOLON)}, {Token(MINUS), Token(RELOP, NT), Token(ID, FREE), Token(LPAREN), Token(NUMBER), Token(LITERAL, CHAR), Token(LITERAL, STRING)}})}, 
            {Token(ID, STATE, "BinOP") , vector<vector<Token>>({{Token(PLUS)}, {Token(MINUS)}, {Token(MULT)}, {Token(DIV)}})}, 
            {Token(ID, STATE, "RelOP") , vector<vector<Token>>({{Token(RELOP, EQ)}, {Token(RELOP, NE)}, {Token(RELOP, LE)}, {Token(RELOP, LT)}, {Token(RELOP, GE)}, {Token(RELOP, GT)}})}, 
            {Token(ID, STATE, "LogOP"), vector<vector<Token>>({{Token(RELOP, AND)}, {Token(RELOP, OR)} })}
        };
        std::map<Token, std::vector<std::vector<Token>>> rules = {
            {Token(ID, STATE, "Program"), {{Token(ID, STATE, "mFunction"), Token(END_OF_FILE)}}},
            {Token(ID, STATE, "Function"), {{Token(ID, STATE, "Type"), Token(ID, FREE), Token(LPAREN), Token(ID, STATE, "ParamTypes"), Token(RPAREN), Token(LBRACKET), Token(ID, STATE, "mVarDeclaration"), Token(ID, STATE, "mStatement"), Token(RBRACKET)},{Token(ID, RESERVED, "void"), Token(ID, FREE), Token(LPAREN), Token(ID, STATE, "ParamTypes"), Token(RPAREN), Token(LBRACKET), Token(ID, STATE, "mVarDeclaration"), Token(ID, STATE, "mStatement"), Token(RBRACKET)}}},
            {Token(ID, STATE, "mFunction"), {{Token(USELESS)}, {Token(ID, STATE, "Function"), Token(ID, STATE, "mFunction")}}},
            {Token(ID, STATE, "mVarDeclaration"), {{Token(USELESS)}, {Token(ID, STATE, "Type"), Token(ID, STATE, "VarDeclaration"), Token(ID, STATE, "commaDeclaration"), Token(SEMICOLON), Token(ID, STATE, "mVarDeclaration")}}},
            {Token(ID, STATE, "commaDeclaration"), {{Token(USELESS)}, {Token(COMMA), Token(ID, STATE, "VarDeclaration"), Token(ID, STATE, "commaDeclaration")}}},
            {Token(ID, STATE, "VarDeclaration"), {{Token(ID, FREE), Token(ID, STATE, "possibleArrDef")}}},
            {Token(ID, STATE, "possibleArrDef"), {{Token(USELESS)}, {Token(LBRACE), Token(NUMBER), Token(RBRACE)}}},
            {Token(ID, STATE, "Type"), {{Token(ID, RESERVED, "char")}, {Token(ID, RESERVED, "int")}}},
            {Token(ID, STATE, "ParamTypes"), {{Token(ID, RESERVED, "void")}, {Token(ID, STATE, "Type"), Token(ID, FREE), Token(ID, STATE, "possibleArr"), Token(ID, STATE, "mParamTypes")}}},
            {Token(ID, STATE, "possibleArr"), {{Token(USELESS)}, {Token(LBRACE), Token(RBRACE)}}},
            {Token(ID, STATE, "mParamTypes"), {{Token(USELESS)}, {Token(COMMA), Token(ID, STATE, "Type"), Token(ID, FREE), Token(ID, STATE, "possibleArr"), Token(ID, STATE, "mParamTypes")}}},
            {Token(ID, STATE, "Statement"), {{Token(ID, RESERVED, "if"), Token(LPAREN), Token(ID, STATE, "Expression"), Token(RPAREN), Token(ID, STATE, "Statement"), Token(ID, STATE, "elseStatement")}, {Token(ID, RESERVED, "while"), Token(LPAREN), Token(ID, STATE, "Expression"), Token(RPAREN), Token(ID, STATE, "Statement")}, {Token(ID, RESERVED, "for"), Token(LPAREN), Token(ID, STATE, "possibleAssign"), Token(SEMICOLON), Token(ID, STATE, "possibleExpression"), Token(SEMICOLON), Token(ID, STATE, "possibleAssign"), Token(RPAREN), Token(ID, STATE, "Statement")}, {Token(ID, RESERVED, "return"), Token(ID, STATE, "possibleExpression"), Token(SEMICOLON)}, {Token(ID, FREE), Token(ID, STATE, "IDdecision"), Token(SEMICOLON)}, {Token(LBRACKET), Token(ID, STATE, "mStatement"), Token(RBRACKET)}, {Token(SEMICOLON)}}},
            {Token(ID, STATE, "elseStatement"), {{Token(USELESS)}, {Token(ID, RESERVED, "else"), Token(ID, STATE, "Statement")}}},
            {Token(ID, STATE, "mStatement"), {{Token(USELESS)}, {Token(ID, STATE, "Statement"), Token(ID, STATE, "mStatement")}}},
            {Token(ID, STATE, "IDdecision"), {{Token(ID, STATE, "possibleIDCall"), Token(ASSIGN), Token(ID, STATE, "Expression")}, {Token(LPAREN), Token(ID, STATE, "possibleExpression_comma"), Token(RPAREN)}}},
            {Token(ID, STATE, "Assign"), {{Token(ID, FREE), Token(ID, STATE, "possibleIDCall"), Token(ASSIGN), Token(ID, STATE, "Expression")}}},
            {Token(ID, STATE, "possibleIDCall"), {{Token(USELESS)}, {Token(LBRACE), Token(ID, STATE, "Expression"), Token(RBRACE)}}},
            {Token(ID, STATE, "possibleAssign"), {{Token(USELESS)}, {Token(ID, STATE, "Assign")}}},
            {Token(ID, STATE, "Expression"), {{Token(MINUS), Token(ID, STATE, "Expression"), Token(ID, STATE, "Operation")}, {Token(RELOP, NT), Token(ID, STATE, "Expression"), Token(ID, STATE, "Operation")}, {Token(ID, FREE), Token(ID, STATE, "IDCall"), Token(ID, STATE, "Operation")}, {Token(LPAREN), Token(ID, STATE, "Expression"), Token(RPAREN), Token(ID, STATE, "Operation")}, {Token(NUMBER), Token(ID, STATE, "Operation")}, {Token(LITERAL, CHAR), Token(ID, STATE, "Operation")}, {Token(LITERAL, STRING), Token(ID, STATE, "Operation")}}},
            {Token(ID, STATE, "Operation"), {{Token(ID, STATE, "BinOP"), Token(ID, STATE, "Expression")}, {Token(ID, STATE, "RelOP"), Token(ID, STATE, "Expression")}, {Token(ID, STATE, "LogOP"), Token(ID, STATE, "Expression")}, {Token(USELESS)}}},
            {Token(ID, STATE, "commaExpression"), {{Token(USELESS)}, {Token(COMMA), Token(ID, STATE, "Expression"), Token(ID, STATE, "commaExpression")}}},
            {Token(ID, STATE, "possibleExpression_comma"), {{Token(USELESS)}, {Token(ID, STATE, "Expression"), Token(ID, STATE, "commaExpression")}}},
            {Token(ID, STATE, "IDCall"), {{Token(USELESS)}, {Token(LBRACE), Token(ID, STATE, "Expression"), Token(ID, STATE, "commaExpression"), Token(RBRACE)}}},
            {Token(ID, STATE, "possibleExpression"), {{Token(USELESS)}, {Token(ID, STATE, "Expression")}}},
            {Token(ID, STATE, "BinOP"), {{Token(PLUS)}, {Token(MINUS)}, {Token(MULT)}, {Token(DIV)}}},
            {Token(ID, STATE, "RelOP"), {{Token(RELOP, EQ)}, {Token(RELOP, NE)}, {Token(RELOP, LE)}, {Token(RELOP, LT)}, {Token(RELOP, GE)}, {Token(RELOP, GT)}}},
            {Token(ID, STATE, "LogOP"), {{Token(RELOP, AND)}, {Token(RELOP, OR)}}},
        };
        vector<string> reservedword = {"else", "if", "while", "return", "for", "char", "void", "int"};
        Token getTokenFromScanner();
        bool is_reservedword(Token &n);
        int isIn(stack<Token> s, Token &n);

    public:
        ParserLL();
        bool process(string);
};
