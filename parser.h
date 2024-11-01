#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "scanner.h"

enum TableKind {
    GOTO,
    ACTION
};

enum types{
    ERROR=-1,
    ACCEPT,
    REDUCE,
    SHIFT,
};

struct Action
{
    int kind=-1;
    int to=-1;
};

struct Table {
    std::vector<std::map<Token, Action>> table_action;
    std::vector<std::map<Token, int>> table_goto;
};

class Parser
{
    private:
        Scanner* scanner;
        Token* lToken;
        Table table;
        vector<vector<std::string>> read_csv(int);
        void generate_table(vector<vector<std::string>>, vector<vector<std::string>>);
        void resolveActionTable(int i, Token *header, int j, std::vector<std::vector<std::string>> &arrAction);
        void addKeyToTable(std::vector<std::vector<std::string>> &arrAction, int i, int j, Token *header);
        Token assignToken(const std::string &tokenType);
        Token getTokenFromScanner();

    public:
        Parser(string);
};
