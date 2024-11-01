#include "parser.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <regex>

Parser::Parser(string input) {
	this->scanner = new Scanner(input);
    this->table = Table();
	vector<vector<string>> arrgoto = read_csv(GOTO);
	vector<vector<string>> arraction = read_csv(ACTION);
	this->generate_table(arrgoto, arraction);
}

void Parser::generate_table(vector<vector<string>>  arrGoto, vector<vector<string>> arrAction) {
    int headerLength = (int) arrAction[0].size();
    Token* actionHeader = new Token[headerLength];
    for(int i=0; i < arrAction.size(); i++) {
        if (i != 0)
            this->table.table_action.push_back(std::map<Token, Action>());
        for(int j=2; j < arrAction[i].size(); j++)
            resolveActionTable(i, actionHeader, j, arrAction);
    }
}

void Parser::resolveActionTable(int i, Token *header, int j, std::vector<std::vector<std::string>> &arrAction)
{
    if (i == 0)
    {
        header[j] = assignToken(arrAction[i][j]);
    }
    else if (i != 0)
    {
        addKeyToTable(arrAction, i, j, header);
    }
}

void Parser::addKeyToTable(std::vector<std::vector<std::string>> &arrAction, int i, int j, Token *header)
{
    Action g;
    if (arrAction[i][j][0] == 'r')
    {
        g = {REDUCE, std::stoi(arrAction[i][j].substr(1))};
    }
    else if (arrAction[i][j].size() > 0)
    {
        g = {SHIFT, std::stoi(arrAction[i][j].substr(1))};
    }
    else
    {
        g = {ERROR, -1};
    }
    this->table.table_action[i-1].insert(std::make_pair(header[j], g));
}

Token Parser::assignToken(const std::string& tokenType) {
    if (tokenType == "charconstant") {
        return Token(LITERAL, CHAR);
    } else if (tokenType == "stringconstant") {
        return Token(LITERAL, STRING);
    } else if (tokenType == "integerconstant") {
        return Token(NUMBER);
    } else if (tokenType == "$") {
        return Token(ID, RESERVED, "$");
    }
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
	else
		file = ifstream("./action.csv");
    
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
		}
        data.push_back(rowData);
    }

    file.close();

    return data;
}
