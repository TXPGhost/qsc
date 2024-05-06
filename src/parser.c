#include "parser.h"
#include "ast/expr.h"
#include "lexer.h"
#include "log.h"
#include "token.h"

#include <stdlib.h>
#include <string.h>

Parser* ParserNew(FILE* file) {
    Parser* parser = malloc(sizeof(Parser));
    Lexer* lexer = LexerNew(file);
    *parser = (Parser){
        .lexer = lexer,
        .next_tok = LexerGetTok(lexer),
        .cur_tok_string = '\0',
        .next_tok_string = '\0',
    };
    memset(parser->cur_tok_string, '\0', TOK_STRING_MAX_LEN + 1);
    memset(parser->next_tok_string, '\0', TOK_STRING_MAX_LEN + 1);
    strcpy(parser->next_tok_string, LexerGetTokString(lexer));
    return parser;
}

void ParserFree(Parser* parser) {
    LexerFree(parser->lexer);
    free(parser);
}

Token ParserGetTok(Parser* parser) {
    Token t = parser->next_tok;
    parser->next_tok = LexerGetTok(parser->lexer);
    strcpy(parser->cur_tok_string, parser->next_tok_string);
    strcpy(parser->next_tok_string, LexerGetTokString(parser->lexer));
    TRACE("ate %s (next %s)", TokenGetKindString(t),
          TokenGetKindString(parser->next_tok));
    return t;
}

Token ParserPeekTok(Parser* parser) {
    return parser->next_tok;
}

Expr* ParserParseP0(Parser* parser);

// Paren, Curly, Bracket
Expr* ParserParseP10(Parser* parser) {
    Token tok = ParserPeekTok(parser);

    // Parse groupings
    if (tok == TokLParen) {
        // Eat paren
        TRACE("Begin Paren...");
        ParserGetTok(parser);

        // Check for unit type
        if (ParserPeekTok(parser) == TokRParen) {
            ParserGetTok(parser);
            ExprTuple tuple = (ExprTuple){.exprs = NULL, .n = 0};
            Expr* expr = malloc(sizeof(Expr));
            *expr = (Expr){.kind = ExprKindTuple, .expr.tuple = tuple};
            return expr;
        }

        Expr* expr = ParserParseP0(parser);
        if (ParserPeekTok(parser) != TokRParen) {
            ERROR("expected RParen");
            return NULL;
        }

        // Eat paren
        ParserGetTok(parser);
        TRACE("End Paren...");

        return expr;
    } else if (tok == TokLCurly) {
        // Eat curly
        TRACE("Begin Curly...");
        ParserGetTok(parser);

        Expr* expr = ParserParseP0(parser);
        if (ParserPeekTok(parser) != TokRCurly) {
            ERROR("expected RCurly");
            return NULL;
        }

        // Eat curly
        ParserGetTok(parser);
        TRACE("End Curly...");

        return expr;
    } else if (tok == TokLBracket) {
        // Eat bracket
        TRACE("Begin Bracket...");
        ParserGetTok(parser);

        Expr* expr = ParserParseP0(parser);
        if (ParserPeekTok(parser) != TokRBracket) {
            ERROR("expected RBracket");
            return NULL;
        }

        // Eat bracket
        ParserGetTok(parser);
        TRACE("End Bracket...");

        return expr;
    }

    return NULL;
}

// Ident, Number, String, Paren
Expr* ParserParseP9(Parser* parser) {
    Expr* expr = malloc(sizeof(Expr));
    Token tok = ParserPeekTok(parser);

    if (tok == TokIdent) {
        TRACE("Parsing Ident...");
        // Eat ident
        ParserGetTok(parser);

        ExprIdent ident;
        int len = strlen(parser->cur_tok_string) + 1;
        ident.name = malloc(len);
        memcpy(ident.name, parser->cur_tok_string, len);

        *expr = (Expr){.kind = ExprKindIdent, .expr.ident = ident};
        return expr;
    } else if (tok == TokNumber) {
        TRACE("Parsing Number...");
        // Eat number
        ParserGetTok(parser);

        // TODO: support floats
        ExprInteger integer;
        integer.value = strtol(parser->cur_tok_string, 0, 10);

        *expr = (Expr){.kind = ExprKindInteger, .expr.integer = integer};
        return expr;
    } else if (tok == TokString) {
        TRACE("Parsing String...");
        // Eat string
        ParserGetTok(parser);

        ExprString string;
        int len = strlen(parser->cur_tok_string) - 1;
        string.value = malloc(len);
        memcpy(string.value, parser->cur_tok_string + 1, len);
        string.value[len] = '\0';

        *expr = (Expr){.kind = ExprKindString, .expr.string = string};
        return expr;
    }

    // Fallback
    return ParserParseP10(parser);
}

// Call
Expr* ParserParseP8(Parser* parser) {
    Expr* expr = ParserParseP9(parser);
    Token tok = ParserPeekTok(parser);

    while (tok == TokLParen) {
        TRACE("Parsing Call...");

        // Parse argument
        Expr* arg = ParserParseP10(parser);

        ExprCall call = (ExprCall){.func = expr, .arg = arg};

        expr = malloc(sizeof(Expr));
        *expr = (Expr){.kind = ExprKindCall, .expr.call = call};

        tok = ParserPeekTok(parser);
    }

    return expr;
}

// Neg
Expr* ParserParseP7(Parser* parser) {
    Token tok = ParserPeekTok(parser);

    if (tok == TokSub) {
        TRACE("Parsing Neg...");

        // Eat negative sign
        ParserGetTok(parser);

        ExprNeg neg = (ExprNeg){.expr = ParserParseP7(parser)};
        Expr* expr = malloc(sizeof(Expr));
        *expr = (Expr){.kind = ExprKindNeg, .expr.neg = neg};
        return expr;
    }

    // Fallback
    return ParserParseP8(parser);
}

// Mul, Div, Mod
Expr* ParserParseP6(Parser* parser) {
    // Parse lhs
    Expr* lhs = ParserParseP7(parser);
    Token tok = ParserPeekTok(parser);

    if (tok == TokMul || tok == TokDiv || tok == TokMod) {
        TRACE("Parsing Mul/Div/Mod...");
        // Eat math
        ParserGetTok(parser);

        // Parse rhs
        Expr* rhs = ParserParseP6(parser);

        // Return expr
        Expr* expr = malloc(sizeof(Expr));
        if (tok == TokMul) {
            ExprMul mul = (ExprMul){.lhs = lhs, .rhs = rhs};
            *expr = (Expr){.kind = ExprKindMul, .expr.mul = mul};
            return expr;
        } else if (tok == TokDiv) {
            ExprDiv div = (ExprDiv){.lhs = lhs, .rhs = rhs};
            *expr = (Expr){.kind = ExprKindDiv, .expr.div = div};
            return expr;
        } else {
            ExprMod mod = (ExprMod){.lhs = lhs, .rhs = rhs};
            *expr = (Expr){.kind = ExprKindMod, .expr.mod = mod};
            return expr;
        }
    }

    // Fallback
    return lhs;
}

// Add, Sub
Expr* ParserParseP5(Parser* parser) {
    // Parse lhs
    Expr* lhs = ParserParseP6(parser);
    Token tok = ParserPeekTok(parser);

    if (tok == TokAdd || tok == TokSub) {
        TRACE("Parsing Add/Sub...");
        // Eat math
        ParserGetTok(parser);

        // Parse rhs
        Expr* rhs = ParserParseP5(parser);

        // Return expr
        Expr* expr = malloc(sizeof(Expr));
        if (tok == TokAdd) {
            ExprAdd add = (ExprAdd){.lhs = lhs, .rhs = rhs};
            *expr = (Expr){.kind = ExprKindAdd, .expr.add = add};
            return expr;
        } else {
            ExprSub sub = (ExprSub){.lhs = lhs, .rhs = rhs};
            *expr = (Expr){.kind = ExprKindSub, .expr.sub = sub};
            return expr;
        }
    }

    // Fallback
    return lhs;
}

// Tuple
Expr* ParserParseP4(Parser* parser) {
    // Parse first expr
    Expr* first = ParserParseP5(parser);
    Token tok = ParserPeekTok(parser);

    if (tok == TokComma) {
        // More exprs follow
        TRACE("Parsing Tuple...");

        // Create expr array
        int len = 1;
        int cap = 4;

        ExprTuple tuple =
            (ExprTuple){.exprs = malloc(sizeof(Expr*) * cap), .n = len};
        tuple.exprs[0] = first;

        // Parse all exprs
        while (tok == TokComma) {
            if (len >= cap) {
                cap *= 2;
                tuple.exprs = realloc(tuple.exprs, sizeof(Expr) * cap);
            }

            // Eat semicolon
            ParserGetTok(parser);

            // Parse next expr
            first = ParserParseP5(parser);
            tok = ParserPeekTok(parser);

            // If the expr is NULL, make it unit expr
            if (first == NULL) {
                first = malloc(sizeof(Expr));
                *first =
                    (Expr){.kind = ExprKindTuple,
                           .expr.tuple = (ExprTuple){.exprs = NULL, .n = 0}};
            }

            // Add to list
            tuple.exprs[len] = first;
            tuple.n = ++len;
        }

        // Return the final expr
        Expr* expr = malloc(sizeof(Expr));
        *expr = (Expr){.kind = ExprKindTuple, .expr.tuple = tuple};
        return expr;
    }

    // Fallback
    return first;
}

// Sup
Expr* ParserParseP3(Parser* parser) {
    // Parse first expr
    Expr* first = ParserParseP4(parser);
    Token tok = ParserPeekTok(parser);

    if (tok == TokBar) {
        // More exprs follow
        TRACE("Parsing Sup...");

        // Create expr array
        int len = 1;
        int cap = 4;

        ExprSup sup = (ExprSup){.exprs = malloc(sizeof(Expr*) * cap), .n = len};
        sup.exprs[0] = first;

        // Parse all exprs
        while (tok == TokBar) {
            if (len >= cap) {
                cap *= 2;
                sup.exprs = realloc(sup.exprs, sizeof(Expr) * cap);
            }

            // Eat semicolon
            ParserGetTok(parser);

            // Parse next expr
            first = ParserParseP5(parser);
            tok = ParserPeekTok(parser);

            // If the expr is NULL, make it unit expr
            if (first == NULL) {
                first = malloc(sizeof(Expr));
                *first = (Expr){.kind = ExprKindSup,
                                .expr.sup = (ExprSup){.exprs = NULL, .n = 0}};
            }

            // Add to list
            sup.exprs[len] = first;
            sup.n = ++len;
        }

        // Return the final expr
        Expr* expr = malloc(sizeof(Expr));
        *expr = (Expr){.kind = ExprKindSup, .expr.sup = sup};
        return expr;
    }

    // Fallback
    return first;
}

// Func
Expr* ParserParseP2(Parser* parser) {
    // Parse arg
    Expr* arg = ParserParseP3(parser);
    Token tok = ParserPeekTok(parser);

    if (tok == TokArrow) {
        TRACE("Parsing Func...");
        // Eat arrow
        ParserGetTok(parser);

        // Parse body
        Expr* body = ParserParseP2(parser);

        // Return expr
        Expr* expr = malloc(sizeof(Expr));
        ExprFunc func = (ExprFunc){.arg = arg, .body = body};
        *expr = (Expr){.kind = ExprKindFunc, .expr.func = func};
        return expr;
    }

    // Fallback
    return arg;
}

// Def, Assn
Expr* ParserParseP1(Parser* parser) {
    // Parse lhs
    Expr* lhs = ParserParseP2(parser);
    Token tok = ParserPeekTok(parser);

    if (tok == TokEquals || tok == TokSetEquals) {
        TRACE("Parsing Def/Assn...");

        // Make sure lhs is an ident
        if (lhs->kind != ExprKindIdent) {
            ERROR("lhs of Def or Assn must be Ident");
            return NULL;
        }

        // Eat Equals/SetEquals
        ParserGetTok(parser);

        // Parse rhs
        Expr* rhs = ParserParseP2(parser);

        // Return expr
        Expr* expr = malloc(sizeof(Expr));
        if (tok == TokEquals) {
            ExprDef def = (ExprDef){.ident = lhs->expr.ident, .expr = rhs};
            *expr = (Expr){.kind = ExprKindDef, .expr.def = def};
            return expr;
        } else {
            ExprAssn assn = (ExprAssn){.ident = lhs->expr.ident, .expr = rhs};
            *expr = (Expr){.kind = ExprKindAssn, .expr.assn = assn};
            return expr;
        }
    }

    // Fallback
    return lhs;
}

// Block
Expr* ParserParseP0(Parser* parser) {
    // Parse first expr
    Expr* first = ParserParseP1(parser);
    Token tok = ParserPeekTok(parser);

    if (tok == TokSemicolon) {
        // More exprs follow
        TRACE("Parsing Semicolon...");

        // Create expr array
        int len = 1;
        int cap = 4;

        ExprBlock block =
            (ExprBlock){.exprs = malloc(sizeof(Expr*) * cap), .n = len};
        block.exprs[0] = first;

        // Parse all exprs
        while (tok == TokSemicolon) {
            if (len >= cap) {
                cap *= 2;
                block.exprs = realloc(block.exprs, sizeof(Expr) * cap);
            }

            // Eat semicolon
            ParserGetTok(parser);

            // Parse next expr
            first = ParserParseP1(parser);
            tok = ParserPeekTok(parser);

            // If the expr is NULL, make it unit expr
            if (first == NULL) {
                first = malloc(sizeof(Expr));
                *first =
                    (Expr){.kind = ExprKindTuple,
                           .expr.tuple = (ExprTuple){.exprs = NULL, .n = 0}};
            }

            // Add to list
            block.exprs[len] = first;
            block.n = ++len;
        }

        // Return the final expr
        Expr* expr = malloc(sizeof(Expr));
        *expr = (Expr){.kind = ExprKindBlock, .expr.block = block};
        return expr;
    }

    // Fallback
    return first;
}

Expr* ParserParse(Parser* parser) {
    // Parse file
    Expr* expr = ParserParseP0(parser);

    // Make sure nothing left over
    Token tok = ParserPeekTok(parser);
    if (tok != TokNone) {
        ERROR("expected EOF, found %s", TokenGetKindString(tok));
    }

    TRACE("Done parsing file.");
    return expr;
}
