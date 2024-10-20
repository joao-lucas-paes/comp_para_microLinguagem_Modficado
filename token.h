#include <iostream>
#include <ctype.h>//Funções de caracteres
#include <string>

using namespace std;

enum Names 
{
    UNDEF=-1,
    ID,
    PLUS,
    MINUS,
    MULT,
    DIV,
    NUMBER,
    LPAREN,
    RPAREN,
    LBRACKET,
    RBRACKET,
    LBRACE,
    RBRACE,
    COMMA,
    SEMICOLON,
    ASSIGN,
    RELOP,
    LITERAL,
    END_OF_FILE
};


enum LITERAL {
    CHAR,
    STRING
};

enum RELOP 
{
    LE,
    LT,
    EQ,
    NE,
    GT,
    GE,
    NT,
    AND,
    OR,
};

class Token 
{
    public: 
        int name;
        int attribute;
        string lexeme;
    
        Token(int name)
        {
            this->name = name;
            attribute = UNDEF;
        }
        
        Token(int name, int attr)
        {
            this->name = name;
            attribute = attr;
        }
};