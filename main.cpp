#include "parserll.h"
#include <sstream>

int main() {
    ParserLL p = ParserLL();
    std::string input = "";
    std::string linha;
    
    // while (std::getline(std::cin, linha)) {
    //     input += linha + "\n";
    // }

    cout << p.process("") << endl;

    return 0;
}
