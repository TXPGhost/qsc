#include "lexer.hpp"
#include <cctype>

Lexer::Lexer(std::istream& istream) : istream(istream) {
    c = -1;
    istream >> nc;
}

void Lexer::nextc() {
    c = nc;
    istream >> nc;
}

Token Lexer::next() {
    while (isspace(c)) {
        nextc();
    }

    switch (c) {
    case '(':
        return LeftParen;
    case ')':
        return RightParen;
    case '{':
        return LeftCurly;
    case '}':
        return RightCurly;
    case '[':
        return LeftBracket;
    case ']':
        return RightBracket;
    case '>':
        if (nc == '=') {
            nextc();
            return GreaterThanEquals;
        }
        return RightAngle;
    case '<':
        if (nc == '=') {
            nextc();
            return LessThanEquals;
        }
        return LeftAngle;
    case ';':
        return Semicolon;
    case ',':
        return Comma;
    case '.':
        return Dot;
    case ':':
        if (nc == '=') {
            nextc();
            return ColonEquals;
        }
        return Colon;
    case '+':
        return Plus;
    case '-':
        return Minus;
    case '*':
        return Asterisk;
    case '/':
        return Slash;
    case '=':
        if (nc == '=') {
            nextc();
            return EqualsEquals;
        }
        return Equals;
    case '!':
        if (nc == '=') {
            next();
            return NotEquals;
        }
        return Not;
    case '&':
        return And;
    case '|':
        return Or;
    }

    std::string tok;

    return EndOfFile;
}
