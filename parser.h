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

struct Goto
{
    int kind=-1;
    int to=-1;
};

struct Table {
    std::map<int, std::map<Token, int>> table_action;
    std::map<int, std::map<Token, Goto>> table_goto;
};

class Parser
{
    private:
        Scanner* scanner;
        Token* lToken;
        std::map<int, std::map<Token, int>> table_action;
        Table table;
        vector<vector<std::string>> read_csv(int);
        void generate_table(vector<vector<std::string>>, vector<vector<std::string>>);

    public:
        Parser(string);
};
