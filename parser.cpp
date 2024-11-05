#include "parser.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <regex>
#include <stack>

template <typename T>
void printTable(const vector<map<Token, T>>& table) {
    for (const map<Token, T>& row : table) {
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

Parser::Parser() {
	this->scanner = new Scanner("");
    this->table = Table();
	vector<vector<string>> arrgoto = read_csv(GOTO);
	vector<vector<string>> arraction = read_csv(ACTION);
    vector<vector<string>> arrrule = read_csv(RULE);
	this->generate_table(arrgoto, arraction);
    this->generate_rule(arrrule);
}

void Parser::generate_rule(vector<vector<string>> rules) { 
    for (int i = 1; i < rules.size(); i++) {
        this->scanner->setInput(rules[i][1]);
        this->rules.push_back(std::make_pair(getTokenFromScanner(),stoi(rules[i][2])));
    }
}

void Parser::generate_table(vector<vector<string>> arrGoto, vector<vector<string>> arrAction) {
    actionTableGenerate(arrAction);
    gotoTableGenerate(arrGoto);
}

void Parser::actionTableGenerate(std::vector<std::vector<std::string>> &arrAction)
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

void Parser::gotoTableGenerate(std::vector<std::vector<std::string>> &arrGoto)
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

void Parser::resolveActionGoto(int i, Token *header, int j, std::vector<std::vector<std::string>> &arrGoto)
{
    if (i == 0) 
        header[j] = Token(ID, USELESS, arrGoto[i][j]);
    else
        this->table.table_goto[i-1].insert(std::make_pair(header[j],std::stoi(arrGoto[i][j])));
}

void Parser::resolveActionTable(int i, Token *header, int j, std::vector<std::vector<std::string>> &arrAction)
{
    if (i == 0)
        header[j] = assignToken(arrAction[i][j]);
    else if (i != 0)
        addKeyToTable(arrAction, i, j, header);
}

void Parser::addKeyToTable(std::vector<std::vector<std::string>> &arrAction, int i, int j, Token *header)
{
    Action g;
    if (arrAction[i][j][0] == 'r')
        if (header[j].lexeme == "$")
            g = {ACCEPT, ACCEPT};
        else
            g = {REDUCE, std::stoi(arrAction[i][j].substr(1))};
    else if (arrAction[i][j].size() > 0) 
        g = {SHIFT, std::stoi(arrAction[i][j].substr(1))};
    else
        g = {ERROR, -1};
    this->table.table_action[i-1].insert(std::make_pair(header[j], g));
}

Token Parser::assignToken(const std::string& tokenType) {
    if (tokenType == "charconstant")
        return Token(LITERAL, CHAR);
    else if (tokenType == "stringconstant")
        return Token(LITERAL, STRING);
    else if (tokenType == "integerconstant")
        return Token(NUMBER);
    else if (tokenType == "$")
        return Token(ID, RESERVED, "$");

    this->scanner->setInput(tokenType);
    return getTokenFromScanner();
}

Token Parser::getTokenFromScanner()
{
    Token *t = this->scanner->nextToken();
    Token copy = *t;
    delete t;
    return copy;
}

vector<vector<string>> Parser::read_csv(int GotoAction) {
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
Parser::process(std::string input) {
    this->scanner->setInput(input);
    stack<int> s;
    s.push(0);

    while(true) {
        Token n = this->getTokenFromScanner();
        if(this->table.table_action[s.top()].find(n) != this->table.table_action[s.top()].end()) {
            Action a = this->table.table_action[s.top()][n];
            if(a.kind == SHIFT) {
                s.push(a.to);
            } else if(a.kind == REDUCE) {
                for(int i = 0; i < this->rules[a.to].second; i++)
                    s.pop();
                s.push(this->table.table_goto[a.to][this->rules[a.to].first]);
            } else if(a.kind == ACCEPT) {
                return true;
            }
        }
    }
}