#include "parserll.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <regex>
#include <stack>


ParserLL::ParserLL() {
}

Token ParserLL::getTokenFromScanner() {
    if(this->scanner->isOut())
        return Token(ID, RESERVED, "$");
    Token *t = this->scanner->nextToken();
    Token copy = *t;
    delete t;
    if(copy.name == END_OF_FILE)
        return Token(ID, RESERVED, "$");
    return copy;
}


bool
ParserLL::process(std::string input) {

}

bool ParserLL::is_reservedword(Token &n) {
    return find(this->reservedWords.begin(), this->reservedWords.end(), n.lexeme) != this->reservedWords.end();
}
