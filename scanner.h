#include "token.h"
#include <vector>

class Scanner 
{
    private: 
        string input;//Armazena o texto de entrada
        int pos;//Posicao atual
        int spos;//posicao inicial
    
    public:
        //Construtor
        Scanner(string);
    
        //Método que retorna o proximo token da entrada
        Token *nextToken();

        bool isChar();

        bool isDiv();

        bool isCommentLine();

        bool isComment();

        Token *idGetter();

        Token *digitGetter();

        Token *operatorGetter();

        Token *operatorCheck(int operator1, int operator2, char value, int attr1 = -1, int attr2 = -1);

        Token *sumAndToken(int name, int attr = -1);

        vector<string> reservedWords = {"else", "if", "while", "return", "for", "char", "void", "int"};

        bool isOut();

        // Metodo para manipular erros
        void lexicalError();

        void setInput(string);

        int getPos() {
            return this->pos;
        }

        string getLex() {
            return this->input.substr(spos, pos - spos);
        }
};
