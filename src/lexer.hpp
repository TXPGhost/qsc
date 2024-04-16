#ifndef LEXER_HPP
#define LEXER_HPP

#include <istream>

enum Token {
    EndOfFile,         // end of file
    Ident,             // an identifier
    Number,            // a number (i.e. 5)
    String,            // a string (i.e. "hello")
    Char,              // a character (i.e. 'x')
    Let,               // `let`
    Return,            // `return`
    If,                // `if`
    Else,              // `else`
    While,             // `while`
    Switch,            // `switch`
    LeftParen,         // `(`
    RightParen,        // `)`
    LeftCurly,         // `{`
    RightCurly,        // `}`
    LeftBracket,       // `[`
    RightBracket,      // `]`
    RightAngle,        // `>`
    LeftAngle,         // `<`
    Semicolon,         // `;`
    Comma,             // `,`
    Dot,               // `.`
    Colon,             // `:`
    Plus,              // `+`
    Minus,             // `-`
    Asterisk,          // `*`
    Slash,             // `/`
    Equals,            // `=`
    EqualsEquals,      // `==`
    GreaterThanEquals, // `>=`
    LessThanEquals,    // `<=`
    Not,               // `!`
    And,               // `&`
    Or,                // `|`
    NotEquals,         // `!=`
    ColonEquals,       // `:=`
};

class Lexer {
  private:
    std::istream& istream; // The input stream
    int c;                 // The current character
    int nc;                // The next character

    void nextc(); // Reads in the next character

  public:
    Lexer(std::istream& istream);

    // Returns the next token, or `EndOfFile` if there is none
    Token next();
};

#endif
