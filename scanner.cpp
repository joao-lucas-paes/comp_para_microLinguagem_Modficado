#include "scanner.h"    

bool isOperator(char input){
    return input == '=' or input == '>' or input == '<' or input == '!' or input == '&' or input == '|';
}

//Construtor
Scanner::Scanner(string input)
{
    this->input = input;

    cout << "Entrada: " << input << endl << "Tamanho: " 
         << input.length() << endl;

    pos = 0;
}

//Método que retorna o próximo token da entrada
Token* 
Scanner::nextToken()
{
    //string lexeme;

    //Consumir espaços em branco
    while (isspace(input[pos]))
        pos++;

    //Verificar os tokens possíveis
    //Fim de arquivo
    if (input[pos] == '\0')
        return sumAndToken(END_OF_FILE);

    //Operadores aritméticos
    else if (input[pos] == '+')
        return sumAndToken(PLUS);

    else if (input[pos] == '-')
        return sumAndToken(MINUS);
        
    else if (input[pos] == '*')
        return sumAndToken(MULT);

    else if (isDiv())
        return sumAndToken(DIV);

    //Parênteses
    else if (input[pos] == '(')
        return sumAndToken(LPAREN);

    else if (input[pos] == ')')
        return sumAndToken(RPAREN);
    
    //Identificadores
    else if (isalpha(input[pos]) or input[pos] == '_')
        return idGetter();

    //Números
    else if (isdigit(input[pos]))
        return digitGetter();
    
    else if (isOperator(input[pos])) {
        cout << "debbuger: " << pos << " " << input[pos] << endl;
        Token* tkn = operatorGetter();
        if (not (tkn->name == UNDEF))
            return tkn;
    }

    lexicalError();
}

bool Scanner::isDiv()
{
    return input[pos] == '/' and input[pos + 1] != '/' and input[pos + 1] != '*';
}

Token *Scanner::operatorGetter(){
    if (input[pos] == '=')
        return operatorCheck(ASSIGN, RELOP, '=', -1, EQ);
    
    else if (input[pos] == '>')
        return operatorCheck(RELOP, RELOP, '=', GT, GE);        
    
    else if (input[pos] == '<') 
        return operatorCheck(RELOP, RELOP, '=', LT, LE);        
    
    else if (input[pos] == '!') 
        return operatorCheck(RELOP, RELOP, '=',  NT, NE);

    else if (input[pos++] == '&' and input[pos++] == '&')
        return new Token(RELOP, AND);
    
    else if (input[pos++] == '|' and input[pos++] == '|')
        return new Token(RELOP, OR);
    
    return new Token(UNDEF);
}

Token *Scanner::operatorCheck(int operator1, int operator2, char value, int attr1, int attr2)
{
    if (input[++pos] == value)
        return sumAndToken(operator2, attr2);
    return new Token(operator1, attr1);
}

Token *Scanner::idGetter()
{
    // lexeme.push_back(input[pos]);
    pos++;
    while (isalnum(input[pos]) || input[pos] == '_')
    {
        // lexeme.push_back(input[pos]);
        pos++;
    }

    return new Token(ID);
}

Token *Scanner::digitGetter()
{
    // lexeme.push_back(input[pos]);
    pos++;
    while (isdigit(input[pos]))
    {
        // lexeme.push_back(input[pos]);
        pos++;
    }

    return new Token(NUMBER);
}

Token* Scanner::sumAndToken(int name, int attr)
{
    pos++;
    return new Token(name, attr);
}

void 
Scanner::lexicalError()
{
    cout << "Token mal formado\n";
    
    exit(EXIT_FAILURE);
}
