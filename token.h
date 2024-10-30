#include <iostream>
#include <ctype.h>//Funções de caracteres
#include <string>

using namespace std;

enum Names {
    USELESS=-2,
    UNDEF,
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
    BREAKLINE,
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

class Token {
    public: 
        int name;
        int attribute;
        string lexeme;

        Token()
        {
            this->name = USELESS;
        }
    
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

        bool operator==(Token &t) {
            if(this->name != ID and t.name != ID)
                return t.name == this->name and t.attribute == this->attribute;
            else
                return t.name == this->name and t.attribute == this->attribute and this->lexeme == t.lexeme;
        }

        string to_string() {
            string token_str;
            token_str = "Token(ID:" + std::to_string(this->name) + ",Attr:" + std::to_string(this->attribute) + ",Lexeme:" + this->lexeme +")";
            return token_str;
        }


};

template<typename Operator>
struct StructIs {
  int name;
  int attribute=0;
  StructIs(Token t): name(t.name), attribute(t.attribute){};
  StructIs(int n, int a): name(n), attribute(a){};
};

enum tokenOperator { Operationis=-1 };
