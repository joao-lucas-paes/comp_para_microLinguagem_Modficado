#include "scanner.h"    

bool isOperator(char input){
    return input == '=' or input == '>' or input == '<' or input == '!' or input == '&' or input == '|';
}

//Construtor
Scanner::Scanner(string input)
{
    this->input = input;
    pos = 0;
    spos = 0;
}

//Metodo que retorna o proximo token da entrada
Token* 
Scanner::nextToken()
{
    //string lexeme;

    //Consumir espacos em branco
    while (isspace(input[pos]))
        pos++;

    //Verificar os tokens possíveis
    spos = pos;
    //Fim de arquivo
    if (input[pos] == '\0') {
        return sumAndToken(END_OF_FILE);
    }

    //Operadores aritmeticos
    else if (input[pos] == '+')
        return sumAndToken(PLUS);

    else if (input[pos] == '-')
        return sumAndToken(MINUS);
        
    else if (input[pos] == '*')
        return sumAndToken(MULT);

    else if (isDiv())
        return sumAndToken(DIV);

    else if (isCommentLine()) {
        while (input[++pos] != '\n');
        return this->nextToken();
    }

    else if (isComment()) {
        while (input[++pos] != '*' or input[pos+1] != '/');
        pos+=2;
        return this->nextToken();
    } 

    //Parenteses
    else if (input[pos] == '(')
        return sumAndToken(LPAREN);

    else if (input[pos] == ')')
        return sumAndToken(RPAREN);
    
    else if (input[pos] == '[')
        return sumAndToken(LBRACKET);

    else if (input[pos] == ']')
        return sumAndToken(RBRACKET);
    
    else if (input[pos] == '{')
        return sumAndToken(LBRACE);

    else if (input[pos] == '}')
        return sumAndToken(RBRACE);
    
    else if (input[pos] == ';')
        return sumAndToken(SEMICOLON);
    
    else if (input[pos] == ',')
        return sumAndToken(COMMA);

    else if (input[pos] == '\'') {

        if (isprint(input[++pos]) and 
            input[pos] != '\\' and 
            input[pos] != '\'' and 
            input[++pos] == '\'')
            return sumAndToken(LITERAL, CHAR);

    }

    else if (input[pos] == '\"') {

        while(isprint(input[++pos]) and 
            input[pos] != '\n' and 
            input[pos] != '"');
        
        if (input[pos] == '\"')
            return sumAndToken(LITERAL, STRING);
    }

    //Identificadores
    else if (isalpha(input[pos]))
        return idGetter();

    //Núumeros
    else if (isdigit(input[pos]))
        return digitGetter();
    
    else if (isOperator(input[pos])) {
        Token* tkn = operatorGetter();
        if (not (tkn->name == UNDEF))
            return tkn;
    }

    // ignora a quebra de linha.
    else if (input[pos] == '\n') {
        pos++;
        return nextToken();
    }

    lexicalError();
    return new Token(-1);
}

bool Scanner::isChar() {
    return isprint(input[pos]);
}

bool Scanner::isDiv() {
    return input[pos] == '/' and input[pos + 1] != '/' and input[pos + 1] != '*';
}

bool Scanner::isCommentLine() {
    return input[pos] == '/' and input[pos + 1] == '/';
}

bool Scanner::isComment() {
    return input[pos] == '/' and input[pos + 1] == '*';
}

Token* Scanner::operatorGetter() {
    if (input[pos] == '=')
        return operatorCheck(ASSIGN, RELOP, '=', -1, EQ);
    
    else if (input[pos] == '>')
        return operatorCheck(RELOP, RELOP, '=', GT, GE);        
    
    else if (input[pos] == '<') 
        return operatorCheck(RELOP, RELOP, '=', LT, LE);        
    
    else if (input[pos] == '!') 
        return operatorCheck(RELOP, RELOP, '=',  NT, NE);

    else if (input[pos] == '&' and input[(++pos)++] == '&')
        return new Token(RELOP, AND);
    
    else if (input[pos] == '|' and input[(++pos)++] == '|')
        return new Token(RELOP, OR);
    
    return new Token(UNDEF);
}

Token* Scanner::operatorCheck(int operator1, int operator2, char value, int attr1, int attr2) {
    if (input[++pos] == value)
        return sumAndToken(operator2, attr2);
    return new Token(operator1, attr1);
}

Token* Scanner::idGetter() {
    // lexeme.push_back(input[pos]);
    pos++;
    while (isalnum(input[pos]) || input[pos] == '_') {
        // lexeme.push_back(input[pos]);
        pos++;
    }

    return new Token(ID, FREE, input.substr(spos, pos - spos + 1));
}

Token* Scanner::digitGetter() {
    // lexeme.push_back(input[pos]);
    pos++;
    while (isdigit(input[pos])) {
        // lexeme.push_back(input[pos]);
        pos++;
    }

    return new Token(NUMBER);
}

Token* 
Scanner::sumAndToken(int name, int attr) {
    pos++;
    return new Token(name, attr);
}

void 
Scanner::lexicalError() {
    cout << "Token " << "'" << input.substr(spos, pos - spos + 1) << "' mal formado\n" << endl;
    exit(EXIT_FAILURE);
}

void
Scanner::setInput(string newInput) {
    this->input = newInput;
    this->pos = this->spos = 0;
}
