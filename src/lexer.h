#ifndef LEXER_H
#define LEXER_H

#include "token.h"

#include <stdio.h>

#define TOK_STRING_MAX_LEN 128

typedef struct {
    FILE* file;
    Token next;
    int next_char;
    char tok_string[TOK_STRING_MAX_LEN + 1];
    int tok_string_len;
} Lexer;

Lexer* LexerNew(FILE* file);
void LexerFree(Lexer* lexer);
int LexerGetChar(Lexer* lexer);
int LexerPeekChar(Lexer* lexer);
Token LexerGetTok(Lexer* lexer);
char* LexerGetTokString(Lexer* lexer);

#endif
