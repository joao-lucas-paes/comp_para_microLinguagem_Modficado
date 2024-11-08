#include "parserlr.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <regex>
#include <stack>

template <typename T>
void printTable(const vector<map<Token, T>>& table) {
    int i = 0;
    for (const map<Token, T>& row : table) {
        cout << "LINE(" << i++ << "): ";
        for (const auto& [key, value] : row) {
            cout << key.to_string() << ":";
            if constexpr (std::is_same_v<T, Action>)
                cout << value.to_string();
            else
                cout << value;
            cout << " | ";
        }
        cout << endl;
    }
}

template <typename ValueType>
void printKeys(const std::map<Token, ValueType>& map) {
    for (const auto& pair : map) {
        std::cout << pair.first.to_string() << std::endl;
    }
}

ParserLR::ParserLR() {
	this->scanner = new Scanner("");
    this->table = Table();
	vector<vector<string>> arrgoto = read_csv(GOTO);
	vector<vector<string>> arraction = read_csv(ACTION);
    vector<vector<string>> arrrule = read_csv(RULE);
	generate_table(arrgoto, arraction);
    generate_rule(arrrule);
}

void ParserLR::generate_rule(vector<vector<string>> rules) { 
    for (int i = 1; i < rules.size(); i++) {
        this->scanner->setInput(rules[i][1]);
        Token t = getTokenFromScanner();
        t.attribute = STATE;
        this->rules.push_back(std::make_pair(t,stoi(rules[i][2])));
    }
}

void ParserLR::generate_table(vector<vector<string>> arrGoto, vector<vector<string>> arrAction) {
    actionTableGenerate(arrAction);
    gotoTableGenerate(arrGoto);
}

void ParserLR::actionTableGenerate(std::vector<std::vector<std::string>> &arrAction)
{
    int headerLength = (int)arrAction[0].size();
    Token *actionHeader = new Token[headerLength];
    for (int i = 0; i < arrAction.size(); i++)
    {
        if (i != 0)
            this->table.table_action.push_back(std::map<Token, Action>());
        for (int j = 2; j < arrAction[i].size(); j++)
            resolveActionTable(i, actionHeader, j, arrAction);
    }
    delete[] actionHeader;
}

void ParserLR::gotoTableGenerate(std::vector<std::vector<std::string>> &arrGoto)
{
    int headerLength = (int)arrGoto[0].size();
    Token *gotoHeader = new Token[headerLength];
    for (int i = 0; i < arrGoto.size(); i++)
    {
        if (i != 0)
            this->table.table_goto.push_back(std::map<Token, int>());
        for (int j = 2; j < arrGoto[i].size(); j++)
            resolveActionGoto(i, gotoHeader, j, arrGoto);
    }
    delete[] gotoHeader;
}

void ParserLR::resolveActionGoto(int i, Token *header, int j, std::vector<std::vector<std::string>> &arrGoto)
{
    if (i == 0) 
        header[j] = Token(ID, STATE, arrGoto[i][j]);
    else {
        int to = std::stoi(arrGoto[i][j]);
        if (to != -1)
            this->table.table_goto[i-1].insert(std::make_pair(header[j], to));
    }
}

void ParserLR::resolveActionTable(int i, Token *header, int j, std::vector<std::vector<std::string>> &arrAction)
{
    if (i == 0) {
        header[j] = assignToken(arrAction[i][j]);
        if(header[j].name == ID) 
            this->reservedWords.push_back(header[j].lexeme);
    }
    else if (i != 0)
        addKeyToTable(arrAction, i, j, header);
}

void ParserLR::addKeyToTable(std::vector<std::vector<std::string>> &arrAction, int i, int j, Token *header) {
    Action g;
    if (arrAction[i][j][0] == 'r')
        if (header[j].lexeme == "$")
            g = {ACCEPT, ACCEPT};
        else
            g = {REDUCE, std::stoi(arrAction[i][j].substr(1))};
    else if (arrAction[i][j].size() > 0) 
        g = {SHIFT, std::stoi(arrAction[i][j].substr(1))};
    else
        return; //g = {ERROR, -1};
    this->table.table_action[i-1].insert(std::make_pair(header[j], g));
}

Token ParserLR::assignToken(const std::string& tokenType) {
    if (tokenType == "charconstant")
        return Token(LITERAL, CHAR);
    else if (tokenType == "stringconstant")
        return Token(LITERAL, STRING);
    else if (tokenType == "integerconstant")
        return Token(NUMBER);
    else if (tokenType == "$")
        return Token(ID, RESERVED, "$");
    else if (tokenType == "ID")
        return Token(ID, FREE);

    this->scanner->setInput(tokenType);
    Token t = getTokenFromScanner();
    if (t.name == ID)
        t.attribute = RESERVED;
    return t;
}

Token ParserLR::getTokenFromScanner() {
    if(this->scanner->isOut())
        return Token(ID, RESERVED, "$");
    Token *t = this->scanner->nextToken();
    Token copy = *t;
    delete t;
    if(copy.name == END_OF_FILE)
        return Token(ID, RESERVED, "$");
    return copy;
}

vector<vector<string>> ParserLR::read_csv(int GotoAction) {
    vector<vector<string>> data;
    ifstream file;

	if (GotoAction == GOTO)
		file = ifstream("./goto.csv");
	else if (GotoAction == ACTION)
		file = ifstream("./action.csv");
    else
        file = ifstream("./rules.csv");
    
    if (!file.is_open()) {
        cerr << "Erro ao abrir o arquivo " << GotoAction << endl;
        return data;
    }
    
    string row;
    while (getline(file, row)) {
        stringstream ss(row);
        string cell;
        vector<string> rowData;

        while (getline(ss, cell, ',')) {
            rowData.push_back(cell);
            if (rowData[rowData.size()-1] == "COMMA")
                rowData[rowData.size()-1] = ",";
            else if(rowData[rowData.size()-1] == "EOF")
                rowData[rowData.size()-1] = "\0";

		}
        data.push_back(rowData);
    }

    file.close();

    return data;
}

bool
ParserLR::process(std::string input) {
    this->scanner->setInput(input);
    stack<int> s;
    s.push(0);
    Token n = this->getTokenFromScanner();
    int pocket = 0;

    while(true) {
        stack<int> sCopy = s;        
        if (n.name == ID and is_reservedword(n)) {
            n.attribute = RESERVED;
        }
        if (checkIfRowHasToken(s, n)) {
            Action a = this->table.table_action[s.top()][n];
            if(a.kind == SHIFT) {
                s.push(a.to);
                n = this->getTokenFromScanner();
                pocket++;
            } else if(a.kind == REDUCE) {
                const auto& rule = this->rules[a.to - 1];
                int popCount = rule.second;

                for (int i = 0; i < popCount; ++i) {
                    s.pop();
                }
                int topState = s.top();
                int gotoState = this->table.table_goto[topState][rule.first];
                s.push(gotoState);
            } else if(a.kind == ACCEPT) {
                return true;
            }
        } else {
            cout << "ERRO EM " << n.to_string() << " nao registrado em " << s.top() << endl;
            exit(0);
        }
    }
}

bool ParserLR::checkIfRowHasToken(stack<int> &s, Token &n)
{
    return this->table.table_action[s.top()].find(n) != this->table.table_action[s.top()].end();
}

bool ParserLR::is_reservedword(Token &n) {
    return find(this->reservedWords.begin(), this->reservedWords.end(), n.lexeme) != this->reservedWords.end();
}
