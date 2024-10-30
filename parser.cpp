#include "parser.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <regex>

Parser::Parser(string input) {
	this->scanner = new Scanner(input);
	vector<vector<string>> arrgoto = read_csv(GOTO);
	vector<vector<string>> arraction = read_csv(ACTION);
	this->generate_table(arrgoto, arraction);
}

void Parser::generate_table(vector<vector<string>>  arrGoto, vector<vector<string>> arrAction) {
    int headerLength = (int) arrGoto[0].size();
    Token* header = new Token[headerLength];
    for(int i=0; i < arrGoto.size(); i++) {
        cout << "[";
        for(int j=0; j < arrGoto[i].size(); j++) {
            cout << arrGoto[i][j] << ", ";
            if(i == 0 and j > 2) {
            }
        }
        cout << "]" << endl;
    }
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
