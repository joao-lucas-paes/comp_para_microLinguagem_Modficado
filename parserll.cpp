#include "parserll.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <regex>
#include <stack>


ParserLL::ParserLL() {
    this->scanner = new Scanner("");
}

Token ParserLL::getTokenFromScanner() {
    if(this->scanner->isOut())
        return Token(ID, RESERVED, "$");
    Token *t = this->scanner->nextToken();
    Token copy = *t;
    delete t;
    if(copy.name == END_OF_FILE)
        return Token(ID, RESERVED, "$");
    if(is_reservedword(copy))
        copy.attribute = RESERVED;
    return copy;
}


bool
ParserLL::process(string input) {
    this->scanner->setInput(input);
    Token n = getTokenFromScanner();
    stack<Token> s;
    s.push(Token(ID, STATE, "Program"));
    
    while(true) {
        int index = isIn(s, n);
        if(n.lexeme == "$" and s.size() <= 1)
            return true;
        if(index == -1) { // nao tem
            return false;
        } else { // tem
            if(s.top().attribute == STATE) {
                Token accessKey = s.top();
                s.pop();
                for(int i = this->rules[accessKey][index].size() - 1; i >= 0; i--)
                    if(this->rules[accessKey][index][i].name != USELESS)
                        s.push(this->rules[accessKey][index][i]);
            } else {
                if(n == s.top()) {
                    s.pop();
                    n = getTokenFromScanner();
                }
            }
        }
    }
}

int ParserLL::isIn(stack<Token> s, Token &n) {
    int i = 0;
    for(const auto &m:this->firstp[s.top()]) {
        if(find(m.begin(), m.end(), n) != m.end())
            return i;
        i++;
    }
    return -1;
}

bool ParserLL::is_reservedword(Token &n) {
    return find(this->reservedWords.begin(), this->reservedWords.end(), n.lexeme) != this->reservedWords.end();
}