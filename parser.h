#include "scanner.h"

class Parser
{
    private:
        Scanner* scanner;
        Token* lToken;
        
        // itens de consumo geral
        void advance();
        void match(int, int);
        void match(int, int, string);
        bool isMatch(int, int);
        bool isMatch(int, int, string);
        void matchArrDef(int);
        bool commaConsumed();
        bool typeConsumed();
        bool isStatement();
        bool isExpression();
        bool isBinOp();
        bool isRelOp();
        bool isLogOp();
        bool isType();

        void goal();

        bool isFunction();

        // declaracao e funcao
        void function();
        void type();
        void ParamTypes();
        bool isVoid();
        void VarDeclaration();

        // statement e as coisas doidas
        void statement();
        void assignExpression();
        void idCall();
        void returnCase();
        void ifCase();
        void forCase();
        void possibleAssign();
        void elseCase();
        void whileCase();
        void expressionParen();
        void expression();
        void sufixExpressiion();
        void prefixExpression();
        void assign();

        void error(string);
        
    public:
        Parser(string);

        void run();
};
