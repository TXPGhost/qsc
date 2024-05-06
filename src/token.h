#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
    TokNone,
    TokIdent,
    TokEquals,
    TokSetEquals,
    TokEqualsEquals,
    TokSemicolon,
    TokNumber,
    TokComma,
    TokLParen,
    TokRParen,
    TokLAngle,
    TokRAngle,
    TokLCurly,
    TokRCurly,
    TokLBracket,
    TokRBracket,
    TokDot,
    TokColon,
    TokBar,
    TokArrow,
    TokString,
    TokChar,
    TokAdd,
    TokSub,
    TokMul,
    TokDiv,
    TokMod,
} Token;

const char* const TokenGetKindString(Token token);

#endif
