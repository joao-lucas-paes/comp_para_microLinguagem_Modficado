#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "scanner.h"
#include <stack>

enum TableKind {
    GOTO,
    ACTION,
    RULE,
};

enum types {
    ERROR=-1,
    ACCEPT,
    REDUCE,
    SHIFT,
};

struct Action
{
    int kind=-1;
    int to=-1;
    string to_string() const {
        return "{kind:" + std::to_string(this->kind) + ",to:" + std::to_string(this->to) + "}";
    }
};

struct Table {
    std::vector<std::map<Token, Action>> table_action;
    std::vector<std::map<Token, int>> table_goto;
};

class ParserLR
{
    private:
        Scanner* scanner;
        Token* lToken;
        Table table;
        vector<std::string> reservedWords;
        vector<vector<std::string>> read_csv(int);
        vector<std::pair<Token, int>> rules;
        void generate_table(vector<vector<std::string>>, vector<vector<std::string>>);
        void generate_rule(vector<vector<std::string>>);
        void actionTableGenerate(std::vector<std::vector<std::string>> &arrAction);
        void gotoTableGenerate(std::vector<std::vector<std::string>> &arrGoto);
        void resolveActionTable(int i, Token *header, int j, std::vector<std::vector<std::string>> &arrAction);
        void resolveActionGoto(int i, Token *header, int j, std::vector<std::vector<std::string>> &arrGoto);
        void addKeyToTable(std::vector<std::vector<std::string>> &arrAction, int i, int j, Token *header);
        Token assignToken(const std::string &tokenType);
        Token getTokenFromScanner();
        bool is_reservedword(Token &n);
        bool checkIfRowHasToken(std::stack<int> &s, Token &n);

    public:
        ParserLR();
        bool process(string);

};
