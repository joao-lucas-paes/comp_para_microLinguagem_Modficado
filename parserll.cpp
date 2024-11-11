#include "parserll.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <regex>
#include <stack>

#define LOGFILE_PATH "./acao.log"
#define LOGSTACK_PATH "./stack.log"

void logStack(const std::stack<Token>& tokens) {
    std::ofstream logFile(LOGSTACK_PATH, std::ios::app);
    if (!logFile.is_open()) {
        throw std::runtime_error("Erro ao abrir o arquivo de log");
    }
    
    std::stack<Token> tempStack = tokens;
    std::vector<Token> reversedTokens;

    while (!tempStack.empty()) {
        reversedTokens.push_back(tempStack.top());
        tempStack.pop();
    }

    for (int it = 0; it < reversedTokens.size(); it++) {
        logFile << reversedTokens[it].to_string() << " ";
    }
    logFile << "\n"; 

    logFile.close(); 
}


void logActionWithRule(const string& action, const vector<Token>& rule) {
    ofstream logFile(LOGFILE_PATH, ios::app);
    if (!logFile.is_open()) {
        throw runtime_error("Erro ao abrir o arquivo de log");
    }


    logFile << action << " -> ";
    for (const auto& r : rule)
        logFile << r.to_string() << " ";

    logFile << "\n";

    logFile.close();
}

void logActionWithConsume(const Token& consumed) {
    ofstream logFile(LOGFILE_PATH, ios::app);
    if (!logFile.is_open()) {
        throw runtime_error("Erro ao abrir o arquivo de log");
    }

    logFile << "** " << consumed.to_string() << " foi consumido.";
    logFile << "\n";

    logFile.close();
}

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
        logStack(s); 

        if(n.lexeme == "$" and s.size() == 1 and n == s.top()) {
            return true;
        }
        
        int index = isIn(s, n);  
        if(index == -1) {  // Token nao está na pilha
            if(n == s.top()) {  // Checa se o topo da pilha corresponde ao token lido
                logActionWithConsume(s.top());
                s.pop();
                n = getTokenFromScanner();
                continue;
            }
            cout << n.to_string() << " nao esta disponivel em " << s.top().to_string() << endl;
            return false;
        } else {  // Token esta na pilha
            Token accessKey = s.top();
            s.pop();
            const auto& rule = this->rules[accessKey][index];
            for(int i = rule.size() - 1; i >= 0; i--) {
                if(rule[i].name != USELESS) {
                    s.push(rule[i]);
                }
            }
            logActionWithRule(accessKey.lexeme, rule);
        }
    }
}

// bool ParserLL::process(string input) {
//     this->scanner->setInput(input);
//     Token n = getTokenFromScanner();
//     stack<Token> s;
//     s.push(Token(ID, RESERVED, "$"));
//     s.push(Token(ID, STATE, "Program"));
//     Token end_p = Token(ID, RESERVED, "$");

//     while (true) {
//         if(s.top() == end_p and n == end_p)
//             return true;
//         else if(s.top().name != ID or s.top().attribute != STATE) {
//             if(s.top() == n) {
//                 s.pop();
//                 n = getTokenFromScanner();
//             } else {
//                 cout << n.to_string() << " nao esta disponivel em " << s.top().to_string() << endl;
//                 return false;
//             }
//         } else {
//             int index = isIn(s, n);
//             if(index != -1) {
//                 Token accessKey = s.top();
//                 s.pop();
//                 const auto& rule = this->rules[accessKey][index];
//                 for(int i = rule.size() - 1; i >= 0; i--) {
//                     if(rule[i].name != USELESS) {
//                         s.push(rule[i]);
//                     }
//                 }
//             } else {
//                 cout << n.to_string() << " nao esta disponivel em " << s.top().to_string() << endl;
//                 return false;
//             }
//         }
//     }
// }

int ParserLL::isIn(stack<Token> &s, Token &n) {
    if(s.top().name != ID or s.top().attribute != STATE)
        return -1;
    int i = 0;
    for(const auto &m:this->firstp[s.top()]) {
        if(find(m.begin(), m.end(), n) != m.end())
            return i;
        i++;
    } // caso nao ache atraves do token lido, talvez seja necessario olhar se o estado 
    // nao eh epsilon.
    Token epsilon = Token(USELESS);
    i = 0;
    for(const auto &m:this->firstp[s.top()]) {
        if(find(m.begin(), m.end(), epsilon) != m.end())
            return i;
        i++;
    } // realmente nao tem

    return -1;
}

bool ParserLL::is_reservedword(Token &n) {
    return n.name == ID and find(this->reservedword.begin(), this->reservedword.end(), n.lexeme) != this->reservedword.end();
}
