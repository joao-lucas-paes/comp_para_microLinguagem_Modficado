#include "token.h"

class Scanner 
{
    private: 
        string input;//Armazena o texto de entrada
        int pos;//Posição atual
    
    public:
    //Construtor
        Scanner(string);
    
        //Método que retorna o próximo token da entrada
        Token *nextToken();

        bool isDiv();

        Token *idGetter();

        Token *digitGetter();

        Token *operatorGetter();

        Token *operatorCheck(int operator1, int operator2, char value, int attr1 = -1, int attr2 = -1);

        Token *sumAndToken(int name, int attr = -1);

        // Método para manipular erros
        void lexicalError();
};
