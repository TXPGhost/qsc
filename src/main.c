#include "lexer.h"
#include "log.h"
#include "parser.h"
#include "token.h"

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

    SET_LOG_LEVEL(LLTrace);

    // Test lexer
    {

        FILE* file = fopen(argv[1], "r");
        Lexer* lexer = LexerNew(file);

        Token tok = LexerGetTok(lexer);
        while (tok != TokNone) {
            TRACE("%s    \t'%s'", TokenGetKindString(tok),
                  LexerGetTokString(lexer));
            tok = LexerGetTok(lexer);
        }

        LexerFree(lexer);
        fclose(file);
    }

    // Test parser
    {
        FILE* file = fopen(argv[1], "r");
        Parser* parser = ParserNew(file);

        Expr* ast = ParserParse(parser);

        fprintf(stdout, "\nPRETTY PRINT\n\n");
        ExprPrettyPrint(stdout, ast);
        fprintf(stdout, "\n");

        fprintf(stdout, "\nAST\n\n");
        ExprASTPrint(stdout, ast);
        fprintf(stdout, "\n\n");

        ParserFree(parser);
        fclose(file);
    }
}
