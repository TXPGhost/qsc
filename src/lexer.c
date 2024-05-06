#include "lexer.h"
#include "log.h"

#include <ctype.h>
#include <stdlib.h>

Lexer* LexerNew(FILE* file) {
    Lexer* lexer = malloc(sizeof(Lexer));
    char firstChar = fgetc(file);
    *lexer = (Lexer){
        .file = file,
        .next = TokNone,
        .next_char = firstChar,
        .tok_string = {firstChar, '\0'},
        .tok_string_len = 1,
    };
    return lexer;
}

void LexerFree(Lexer* lexer) {
    free(lexer);
}

int LexerGetChar(Lexer* lexer) {
    int c = lexer->next_char;
    lexer->next_char = fgetc(lexer->file);

    if (lexer->tok_string_len < TOK_STRING_MAX_LEN) {
        lexer->tok_string[lexer->tok_string_len + 1] = '\0';
        lexer->tok_string[lexer->tok_string_len] = c;
        lexer->tok_string_len++;
    } else {
        WARN("token string exceeded TOK_STRING_MAX_LEN (%d)",
             TOK_STRING_MAX_LEN);
    }

    return c;
}

int LexerPeekChar(Lexer* lexer) {
    return lexer->next_char;
}

Token LexerGetTok(Lexer* lexer) {
    // Reset token string
    lexer->tok_string[0] = '\0';
    lexer->tok_string_len = 0;

    // Eat whitespace
    char c = LexerGetChar(lexer);
    while (isspace(c)) {
        c = LexerGetChar(lexer);
    }

    // Reset token string
    lexer->tok_string[0] = c;
    lexer->tok_string[1] = '\0';
    lexer->tok_string_len = 1;

    // Parse token
    switch (c) {
    case EOF:
        return TokNone;
    case '=':
        if (LexerPeekChar(lexer) == '=') {
            LexerGetChar(lexer);
            return TokEqualsEquals;
        }
        return TokEquals;
    case '+':
        return TokAdd;
    case '-':
        if (LexerPeekChar(lexer) == '>') {
            LexerGetChar(lexer);
            return TokArrow;
        }
        return TokSub;
    case '*':
        return TokMul;
    case '/':
        return TokDiv;
    case '&':
        return TokMod;
    case ',':
        return TokComma;
    case ':':
        if (LexerPeekChar(lexer) == '=') {
            LexerGetChar(lexer);
            return TokSetEquals;
        }
        return TokColon;
    case ';':
        return TokSemicolon;
    case '|':
        return TokBar;
    case '(':
        return TokLParen;
    case ')':
        return TokRParen;
    case '{':
        return TokLCurly;
    case '}':
        return TokRCurly;
    case '<':
        return TokLAngle;
    case '>':
        return TokRAngle;
    case '[':
        return TokLBracket;
    case ']':
        return TokRBracket;
    }

    // Eat whitespace
    while (c == '_') {
        // Eat leading underscores
        c = LexerGetChar(lexer);
    }

    if (isdigit(c)) {
        // Parse number
        while (isdigit(c) || c == '_' || c == '.') {
            c = LexerGetChar(lexer);
        }
        lexer->tok_string[lexer->tok_string_len - 1] = '\0';
        return TokNumber;
    } else if (isalpha(c) || c == '_') {
        // Parse ident
        while (isalpha(c) || c == '_') {
            c = LexerGetChar(lexer);
        }
        lexer->tok_string[lexer->tok_string_len - 1] = '\0';
        return TokIdent;
    } else if (c == '\"') {
        // Parse string
        c = LexerGetChar(lexer);
        while (c != '\"') {
            c = LexerGetChar(lexer);
        }
        return TokString;
    } else {
        // Unknown token
        ERROR("unexpected character: '%c'", c);
        return LexerGetTok(lexer);
    }
}

char* LexerGetTokString(Lexer* lexer) {
    return lexer->tok_string;
}
