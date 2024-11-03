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

enum IDTYPES
{
    STATE=-1,
    FREE,
    RESERVED
};

class Token {
    public: 
        int name;
        int attribute;
        string lexeme;

        Token(): name(USELESS) {}
    
        Token(int n) : name(n), attribute(UNDEF){}
        
        Token(int name, int attr) : name(name), attribute(attr){}

        Token(int name, int attr, string lex) : name(name), attribute(attr), lexeme(lex){}

        bool operator==(Token &t) {
            if(this->name != ID)
                return t.name == this->name and t.attribute == this->attribute;
            else
                return t.name == this->name and t.attribute == this->attribute and this->lexeme == t.lexeme;
        }

        bool operator<(const Token& t) const {
            if (this->attribute != t.attribute)
                return this->attribute < t.attribute;
            else if (this->name != t.name)
                return this->name < t.name;
            return this->lexeme < t.lexeme;
        }

        string to_string() const {
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
