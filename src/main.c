#include "log.h"
#include "parser.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        ERROR("usage: qsc <file>");
        return 1;
    }

    FILE* input = fopen(argv[1], "r");
    if (input == NULL) {
        ERROR("unable to read input file \"%s\"", argv[1]);
        return 1;
    }

    FILE* file = fopen(argv[1], "r");
    Parser* parser = ParserNew(file);

    Expr* ast = ParserParse(parser);
    fprintf(stdout, "\nPARSED EXPR\n\n");
    ExprPrettyPrint(stdout, ast);
    fprintf(stdout, "\n\n");

    ParserFree(parser);
    fclose(file);
}
