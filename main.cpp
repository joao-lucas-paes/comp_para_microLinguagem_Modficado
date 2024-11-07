#include "parser.h"
#include <sstream>

int main() {
    Parser p = Parser();
    std::string input = "";
    std::string linha;
    
    while (std::getline(std::cin, linha)) {
        input += linha + "\n";
    }

    cout << p.process(input) << endl;

    return 0;
}
