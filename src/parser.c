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
    return t;
}

Token ParserPeekTok(Parser* parser) {
    return parser->next_tok;
}

Expr* ParserParseP0(Parser* parser);

// Ident, Number, String, Paren
Expr* ParserParseP9(Parser* parser) {
    INFO("          Parsing P9...");

    Expr* expr = malloc(sizeof(Expr));
    Token tok = ParserGetTok(parser);
    if (tok == TokIdent) {
        ExprIdent ident;
        int len = strlen(parser->cur_tok_string) + 1;
        ident.name = malloc(len);
        memcpy(ident.name, parser->cur_tok_string, len);

        *expr = (Expr){.kind = ExprKindIdent, .expr.ident = ident};
        return expr;
    } else if (tok == TokNumber) {
        // TODO: support floats
        ExprInteger integer;
        integer.value = strtol(parser->cur_tok_string, 0, 10);

        *expr = (Expr){.kind = ExprKindInteger, .expr.integer = integer};
        return expr;
    } else if (tok == TokString) {
        ExprString string;
        int len = strlen(parser->cur_tok_string) - 1;
        string.value = malloc(len);
        memcpy(string.value, parser->cur_tok_string + 1, len);
        string.value[len] = '\0';

        *expr = (Expr){.kind = ExprKindString, .expr.string = string};
        return expr;
    } else {
        ERROR("expected Ident, Number, String, or LParen");
        return NULL;
    }

    INFO("DONE");
}

// Call
Expr* ParserParseP8(Parser* parser) {
    INFO("         Parsing P8...");

    Expr* func = ParserParseP9(parser);
    Token tok = ParserGetTok(parser);

    if (tok == TokLParen) {
        // Parse argument
        Expr* arg = ParserParseP0(parser);

        ExprCall call = (ExprCall){.func = func, .arg = arg};

        Expr* expr = malloc(sizeof(Expr));
        *expr = (Expr){.kind = ExprKindCall, .expr.call = call};
        return expr;
    }

    // Fallback
    return func;
}

// Neg
Expr* ParserParseP7(Parser* parser) {
    INFO("        Parsing P7...");

    Expr* expr = ParserParseP8(parser);
    Token tok = ParserGetTok(parser);

    if (tok == TokSub) {
        ExprNeg neg = (ExprNeg){.expr = expr};
        Expr* expr = malloc(sizeof(Expr));
        *expr = (Expr){.kind = ExprKindNeg, .expr.neg = neg};
        return expr;
    }

    // Fallback
    return expr;
}

// Mul, Div, Mod
Expr* ParserParseP6(Parser* parser) {
    INFO("       Parsing P6...");

    // Parse lhs
    Expr* lhs = ParserParseP7(parser);
    Token tok = ParserPeekTok(parser);

    if (tok == TokMul || tok == TokDiv || tok == TokMod) {
        // Eat math
        ParserGetTok(parser);

        // Parse rhs
        Expr* rhs = ParserParseP7(parser);

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
    INFO("      Parsing P5...");

    // Parse lhs
    Expr* lhs = ParserParseP6(parser);
    Token tok = ParserPeekTok(parser);

    if (tok == TokAdd || tok == TokSub) {
        // Eat math
        ParserGetTok(parser);

        // Parse rhs
        Expr* rhs = ParserParseP6(parser);

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
    INFO("     Parsing P4...");

    // Parse first expr
    Expr* first = ParserParseP5(parser);
    Token tok = ParserPeekTok(parser);

    if (tok == TokBar) {
        // More exprs follow

        // Create expr array
        int len = 0;
        int cap = 4;

        ExprTuple tuple =
            (ExprTuple){.exprs = malloc(sizeof(Expr*) * cap), .n = len};
        tuple.exprs[0] = first;

        // Parse all exprs
        while (tok == TokComma) {
            tuple.exprs[len] = first;

            if (++len >= cap) {
                cap *= 2;
                tuple.exprs = realloc(tuple.exprs, sizeof(Expr) * cap);
            }

            // Eat semicolon
            ParserGetTok(parser);

            // Parse next expr
            first = ParserParseP5(parser);
            tok = ParserPeekTok(parser);
            tuple.n = len;
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
    INFO("    Parsing P3...");

    // Parse first expr
    Expr* first = ParserParseP4(parser);
    Token tok = ParserPeekTok(parser);

    if (tok == TokBar) {
        // More exprs follow

        // Create expr array
        int len = 0;
        int cap = 4;

        ExprSup sup = (ExprSup){.exprs = malloc(sizeof(Expr*) * cap), .n = len};
        sup.exprs[0] = first;

        // Parse all exprs
        while (tok == TokBar) {
            sup.exprs[len] = first;

            if (++len >= cap) {
                cap *= 2;
                sup.exprs = realloc(sup.exprs, sizeof(Expr) * cap);
            }

            // Eat semicolon
            ParserGetTok(parser);

            // Parse next expr
            first = ParserParseP4(parser);
            tok = ParserPeekTok(parser);
            sup.n = len;
        }

        // Return the final expr
        Expr* expr = malloc(sizeof(Expr));
        *expr = (Expr){.kind = ExprKindSup, .expr.sup = sup};
        return expr;
    }

    // Fallback
    return first;
}

// Def, Assn
Expr* ParserParseP2(Parser* parser) {
    INFO("   Parsing P2...");

    // Parse lhs
    Expr* lhs = ParserParseP3(parser);
    Token tok = ParserPeekTok(parser);

    if (tok == TokEquals || tok == TokSetEquals) {
        // Make sure lhs is an ident
        if (lhs->kind != ExprKindIdent) {
            ERROR("lhs of Def or Assn must be Ident");
            return NULL;
        }

        // Eat Equals
        ParserGetTok(parser);

        // Parse rhs
        Expr* rhs = ParserParseP3(parser);

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
Expr* ParserParseP1(Parser* parser) {
    INFO("  Parsing P1...");

    // Parse first expr
    Expr* first = ParserParseP2(parser);
    Token tok = ParserPeekTok(parser);

    if (tok == TokSemicolon) {
        // More exprs follow

        // Create expr array
        int len = 0;
        int cap = 4;

        ExprBlock block =
            (ExprBlock){.exprs = malloc(sizeof(Expr*) * cap), .n = len};
        block.exprs[0] = first;

        // Parse all exprs
        while (tok == TokSemicolon) {
            block.exprs[len] = first;

            if (++len >= cap) {
                cap *= 2;
                block.exprs = realloc(block.exprs, sizeof(Expr) * cap);
            }

            // Eat semicolon
            ParserGetTok(parser);

            // Parse next expr
            first = ParserParseP2(parser);
            tok = ParserPeekTok(parser);
            block.n = len;
        }

        // Return the final expr
        Expr* expr = malloc(sizeof(Expr));
        *expr = (Expr){.kind = ExprKindBlock, .expr.block = block};
        return expr;
    }

    // Fallback
    return first;
}

// Paren, Curly, Bracket
Expr* ParserParseP0(Parser* parser) {
    INFO(" Parsing P0...");
    Token tok = ParserPeekTok(parser);

    // Parse groupings
    if (tok == TokLParen) {
        Expr* expr = ParserParseP1(parser);
        if (ParserPeekTok(parser) != TokRParen) {
            ERROR("expected RParen");
            return NULL;
        }
        return expr;
    } else if (tok == TokLCurly) {
        Expr* expr = ParserParseP1(parser);
        if (ParserPeekTok(parser) != TokRCurly) {
            ERROR("expected RCurly");
            return NULL;
        }
        return expr;
    } else if (tok == TokLBracket) {
        Expr* expr = ParserParseP1(parser);
        if (ParserPeekTok(parser) != TokRBracket) {
            ERROR("expected RBracket");
            return NULL;
        }
        return expr;
    }

    // Fallback
    return ParserParseP1(parser);
}

Expr* ParserParse(Parser* parser) {
    INFO("Parsing File...");
    return ParserParseP0(parser);
    INFO("Done parsing file.");
}
