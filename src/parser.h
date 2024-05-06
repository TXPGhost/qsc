#ifndef PARSER_H
#define PARSER_H

#include "ast/expr.h"
#include "lexer.h"

typedef struct {
    Lexer* lexer;
    Token next_tok;
    char cur_tok_string[TOK_STRING_MAX_LEN];
    char next_tok_string[TOK_STRING_MAX_LEN];
} Parser;

Parser* ParserNew(FILE* file);
void ParserFree(Parser* parser);
Expr* ParserParse(Parser* parser);

#endif
