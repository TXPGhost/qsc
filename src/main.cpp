#include <fstream>
#include <iostream>

#include "lexer.hpp"

int main(int argc, char* argv[]) {
    std::string filename = "../test/test.qt";

    std::fstream testfile;
    testfile.open(filename);

    if (!testfile.is_open()) {
        std::cerr << "Could not open file \"" << filename << "\"" << std::endl;
        return 1;
    }

    Lexer lexer(testfile);
    Token tok = lexer.next();
    while (tok != EndOfFile) {
        std::cout << tok << std::endl;
        tok = lexer.next();
    }

    testfile.close();

    return 0;
}
