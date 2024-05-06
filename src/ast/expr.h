#ifndef EXPR_H
#define EXPR_H

#include <stdio.h>

typedef struct Expr Expr;

// name (identifier)
typedef struct {
    char* name;
} ExprIdent;

// value (integer)
typedef struct {
    long value;
} ExprInteger;

// "value" (string)
typedef struct {
    char* value;
} ExprString;

// label: expr
typedef struct {
    ExprIdent label;
    Expr* expr;
} ExprLabel;

// ident = expr
typedef struct {
    ExprIdent ident;
    Expr* expr;
} ExprDef;

// ident := expr
typedef struct {
    ExprIdent ident;
    Expr* expr;
} ExprAssn;

// { exprs[0]; exprs[1]; ...; exprs[n-1] }
typedef struct {
    Expr** exprs;
    int n;
} ExprBlock;

// exprs[0], exprs[1], ..., exprs[n-1]
typedef struct {
    Expr** exprs;
    int n;
} ExprTuple;

// exprs[0] | exprs[1] | ... | exprs[n-1]
typedef struct {
    Expr** exprs;
    int n;
} ExprSup;

// arg -> body
typedef struct {
    Expr* arg;
    Expr* body;
} ExprFunc;

// func(arg)
typedef struct {
    Expr* func;
    Expr* arg;
} ExprCall;

// lhs + rhs
typedef struct {
    Expr* lhs;
    Expr* rhs;
} ExprAdd;

// lhs - rhs
typedef struct {
    Expr* lhs;
    Expr* rhs;
} ExprSub;

// lhs * rhs
typedef struct {
    Expr* lhs;
    Expr* rhs;
} ExprMul;

// lhs / rhs
typedef struct {
    Expr* lhs;
    Expr* rhs;
} ExprDiv;

// lhs % rhs
typedef struct {
    Expr* lhs;
    Expr* rhs;
} ExprMod;

// -expr
typedef struct {
    Expr* expr;
} ExprNeg;

typedef enum {
    ExprKindIdent,
    ExprKindInteger,
    ExprKindString,
    ExprKindLabel,
    ExprKindDef,
    ExprKindAssn,
    ExprKindBlock,
    ExprKindTuple,
    ExprKindSup,
    ExprKindFunc,
    ExprKindCall,
    ExprKindAdd,
    ExprKindSub,
    ExprKindMul,
    ExprKindDiv,
    ExprKindMod,
    ExprKindNeg,
} ExprKind;

typedef struct Expr {
    ExprKind kind;
    union {
        ExprIdent ident;
        ExprInteger integer;
        ExprString string;
        ExprLabel label;
        ExprDef def;
        ExprAssn assn;
        ExprBlock block;
        ExprTuple tuple;
        ExprSup sup;
        ExprFunc func;
        ExprCall call;
        ExprAdd add;
        ExprSub sub;
        ExprMul mul;
        ExprDiv div;
        ExprMod mod;
        ExprNeg neg;
    } expr;
} Expr;

void ExprASTPrint(FILE* file, Expr* expr);
void ExprPrettyPrint(FILE* file, Expr* expr);

#endif
