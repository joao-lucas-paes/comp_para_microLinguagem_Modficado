#include "parser.h"
#include <fstream>

std::string readFileWithNull(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Arquivo indisponivel, tem certeza que o arquivos esta disponivel?");
    }

    file.seekg(0, std::ios::end);
    std::streamsize fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    std::string content(fileSize, '\0');
    if (!file.read(&content[0], fileSize)) {
        throw std::runtime_error("Erro ao ler o arquivo.");
    }

    content += '\0';

    return content;
}

Parser::Parser(string input)
{
	scanner = new Scanner(readFileWithNull(input));
    advance();
}

void
Parser::run()
{
    goal();
}

void
Parser::advance()
{
	lToken = scanner->nextToken();
}

void
Parser::match(int t, int attr=-1)
{	
	if (lToken->name == t and lToken->attribute == attr)
		advance();
	else
		error("Erro inesperado");
}

void
Parser::match(int t, int attr, string lexeme)
{	
	if (lToken->name == t and lToken->attribute == attr and lToken->lexeme == lexeme)
		advance();
	else
		error("Erro inesperado");
}

bool
Parser::isMatch(int t, int attr=-1)
{	
	if (lToken->name == t and lToken->attribute == attr)
		return true;
	else
		return false;
}

bool
Parser::isMatch(int t, int attr, string lexeme)
{	
	if (lToken->name == t and lToken->attribute == attr and lToken->lexeme == lexeme)
		return true;
	else
		return false;
}

void 
Parser::goal() {
    while (isFunction()) {
		function();
    }
	match(END_OF_FILE);
	cout << "entrada aceita" << endl;
}

bool Parser::isFunction()
{
    return isVoid() or isType();
}

void
Parser::error(string str)
{
	cout << str << endl;
	throw exception();
	exit(EXIT_FAILURE);
}

void
Parser::type() {
	if(isType())
		return advance();
	error("Era esperado um tipo ('int','char'), mas recebeu-se: " + lToken->to_string());
}

void
Parser::ParamTypes() {
    if (isVoid())
    {
        match(ID, RESERVED, "void");
    }
    else
    {
        do { //pegunta se comma vai ser consumido
			type(); // int ou char
			match(ID, FREE); // ID
			matchArrDef(USELESS); // ve se tem '[', caso tenha, da match no ].			
		} while(commaConsumed());
    }
}

bool Parser::isVoid() {
    return isMatch(ID, RESERVED, "void");
}

void
Parser::matchArrDef(int t) {
	if(lToken->name == LBRACKET) {
		advance();
		if(t != USELESS) 
			match(t);
		match(RBRACKET);
	}
}

bool
Parser::commaConsumed() {
	if (lToken->name == COMMA) {
		advance();
		return true;
	}
	return false;
}

void Parser::VarDeclaration() {
	match(ID, FREE);
	matchArrDef(NUMBER);
}

bool
Parser::typeConsumed() {
    if (isType())
    {
        advance();
		return true;
    }
    return false;
}

bool Parser::isType()
{
    return isMatch(ID, RESERVED, "int") or isMatch(ID, RESERVED, "char");
}

void 
Parser::function() {
	if(isType() or isVoid()) {
		advance();
		match(ID, FREE);
		match(LPAREN);
		ParamTypes();
		match(RPAREN);
		match(LBRACE);
		
		while(typeConsumed()) {
			do {
				VarDeclaration();
			} while(commaConsumed());
			match(SEMICOLON);
		}

		while(isStatement()) 
			statement();
		match(RBRACE);
	}
}

void
Parser::statement() {
	if(isMatch(ID, RESERVED, "if")) {
        ifCase();
    } else if(isMatch(ID, RESERVED, "while")) {
        whileCase();
    } else if(isMatch(ID, RESERVED, "for")) {
        forCase();
    } else if(isMatch(ID, RESERVED, "return")) {
        returnCase();
    } else if(isMatch(ID, FREE)) {
		advance();
		if(isMatch(LPAREN)) {
            idCall();
			match(SEMICOLON);
		} else {
            assignExpression();
			match(SEMICOLON);
        }
	} else if(isMatch(SEMICOLON)) {
		advance();
	} else {
		match(LBRACE);
		while(isStatement()) statement();
		match(RBRACE);
	}
}

void Parser::idCall()
{
    advance();
    if (isExpression())
        do
        {
            expression();
        } while (commaConsumed());
	match(RPAREN);
}

void Parser::returnCase()
{
    advance();
    if (isExpression())
        expression();
    match(SEMICOLON);
}

void Parser::ifCase()
{
    advance();
    expressionParen();
    statement();
    if (isMatch(ID, RESERVED, "else"))
    {
        elseCase();
    }
}

void Parser::elseCase()
{
    advance();
    statement();
}

void Parser::whileCase()
{
    advance();
    expressionParen();
    statement();
}

void Parser::forCase()
{
    advance();
    match(LPAREN);
    possibleAssign();
    match(SEMICOLON);
	if(isExpression())
    	expression();
    match(SEMICOLON);
    possibleAssign();
    match(RPAREN);
    statement();
}

void Parser::possibleAssign()
{
    if (isMatch(ID, FREE))
        assign();
}

bool
Parser::isExpression() {
	return isMatch(RELOP, NT) or isMatch(ID, FREE) or
	isMatch(LPAREN) or isMatch(NUMBER) or isMatch(LITERAL, CHAR) or
	isMatch(LITERAL, STRING) or isMatch(MINUS);
}

void
Parser::expression() {
    prefixExpression();
    sufixExpressiion();
}

void Parser::sufixExpressiion()
{
    if (isBinOp() or isRelOp() or isLogOp())
    {
        advance();
        expression();
    }
}

void Parser::prefixExpression()
{
    if (isMatch(MINUS))
    {
        advance();
        expression();
    }
    else if (isMatch(RELOP, NT))
    {
        advance();
        expression();
    }
    else if (isMatch(ID, FREE))
    {
        advance();
        if (isMatch(LPAREN))
        {
            idCall();
        }
        else if (isMatch(LBRACKET))
        {
            advance();
            expression();
            match(RBRACKET);
        }
    }
    else if (isMatch(LPAREN))
    {
        advance();
        expression();
        match(RPAREN);
    }
    else
    { // Literal, string/char ou number
        advance();
    }
}

bool
Parser::isBinOp() {
	return isMatch(PLUS) or isMatch(MINUS) or
	isMatch(MULT) or isMatch(DIV);
}

bool
Parser::isRelOp() {
	return isMatch(RELOP, EQ) or isMatch(RELOP, NE) or
	isMatch(RELOP, GT) or isMatch(RELOP, GE) or
	isMatch(RELOP, LT) or isMatch(RELOP, LE);
}

bool
Parser::isLogOp() {
	return isMatch(RELOP, AND) or isMatch(RELOP, OR);
}

void 
Parser::assign() {
	match(ID, FREE);
	assignExpression();
}

void Parser::assignExpression()
{
    if (isMatch(LBRACKET))
    {
        advance();
        match(NUMBER);
        match(RBRACKET);
    }
    match(ASSIGN);
    expression();
}

void Parser::expressionParen()
{
    match(LPAREN);
    expression();
    match(RPAREN);
}

bool
Parser::isStatement() {
	return isMatch(ID, RESERVED, "if") or isMatch(ID, RESERVED, "while") or
	isMatch(ID, RESERVED, "return") or isMatch(ID, RESERVED, "for") or
	isMatch(ID, FREE) or isMatch(SEMICOLON) or isMatch(LBRACE);
}