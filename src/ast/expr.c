#include "expr.h"

#include <stdbool.h>
#include <stdio.h>

int __expr_pretty_print_indent_level = 0;

void ExprASTPrintIndent(FILE* file) {
    for (int i = 0; i < __expr_pretty_print_indent_level; i++) {
        fprintf(file, "··");
    }
}

void ExprASTPrint(FILE* file, Expr* expr) {
    switch (expr->kind) {
    case ExprKindIdent:
        ExprASTPrintIndent(file);
        fprintf(file, "%s\n", expr->expr.ident.name);
        break;
    case ExprKindInteger:
        ExprASTPrintIndent(file);
        fprintf(file, "%ld\n", expr->expr.integer.value);
        break;
    case ExprKindString:
        ExprASTPrintIndent(file);
        fprintf(file, "\"%s\"\n", expr->expr.string.value);
        break;
    case ExprKindLabel:
        ExprASTPrintIndent(file);
        fprintf(file, "LABEL\n");
        __expr_pretty_print_indent_level++;
        ExprASTPrintIndent(file);
        fprintf(file, "LABEL %s\n", expr->expr.label.label.name);
        ExprASTPrintIndent(file);
        fprintf(file, "EXPR\n");
        __expr_pretty_print_indent_level++;
        ExprASTPrint(file, expr->expr.label.expr);
        __expr_pretty_print_indent_level--;
        __expr_pretty_print_indent_level--;
        break;
    case ExprKindDef:
        ExprASTPrintIndent(file);
        fprintf(file, "DEF\n");
        __expr_pretty_print_indent_level++;
        ExprASTPrintIndent(file);
        fprintf(file, "IDENT %s\n", expr->expr.def.ident.name);
        ExprASTPrintIndent(file);
        fprintf(file, "EXPR\n");
        __expr_pretty_print_indent_level++;
        ExprASTPrint(file, expr->expr.def.expr);
        __expr_pretty_print_indent_level--;
        __expr_pretty_print_indent_level--;
        break;
    case ExprKindAssn:
        ExprASTPrintIndent(file);
        fprintf(file, "ASSN\n");
        __expr_pretty_print_indent_level++;
        ExprASTPrintIndent(file);
        fprintf(file, "IDENT %s", expr->expr.def.ident.name);
        fprintf(file, "EXPR\n");
        __expr_pretty_print_indent_level++;
        ExprASTPrint(file, expr->expr.def.expr);
        __expr_pretty_print_indent_level--;
        __expr_pretty_print_indent_level--;
        break;
    case ExprKindBlock:
        ExprASTPrintIndent(file);
        fprintf(file, "BLOCK\n");
        __expr_pretty_print_indent_level++;
        for (int i = 0; i < expr->expr.block.n; i++) {
            ExprASTPrint(file, expr->expr.block.exprs[i]);
        }
        __expr_pretty_print_indent_level--;
        break;
    case ExprKindTuple:
        ExprASTPrintIndent(file);
        fprintf(file, "TUPLE\n");
        __expr_pretty_print_indent_level++;
        for (int i = 0; i < expr->expr.tuple.n; i++) {
            ExprASTPrint(file, expr->expr.tuple.exprs[i]);
        }
        __expr_pretty_print_indent_level--;
        break;
    case ExprKindSup:
        ExprASTPrintIndent(file);
        fprintf(file, "SUP\n");
        __expr_pretty_print_indent_level++;
        for (int i = 0; i < expr->expr.sup.n; i++) {
            ExprASTPrint(file, expr->expr.sup.exprs[i]);
        }
        __expr_pretty_print_indent_level--;
        break;
    case ExprKindFunc:
        ExprASTPrintIndent(file);
        fprintf(file, "FUNC\n");
        __expr_pretty_print_indent_level++;
        ExprASTPrintIndent(file);
        fprintf(file, "ARG\n");
        __expr_pretty_print_indent_level++;
        ExprASTPrint(file, expr->expr.func.arg);
        __expr_pretty_print_indent_level--;
        ExprASTPrintIndent(file);
        fprintf(file, "BODY\n");
        __expr_pretty_print_indent_level++;
        ExprASTPrint(file, expr->expr.func.body);
        __expr_pretty_print_indent_level--;
        __expr_pretty_print_indent_level--;
        break;
    case ExprKindCall:
        ExprASTPrintIndent(file);
        fprintf(file, "CALL\n");
        __expr_pretty_print_indent_level++;
        ExprASTPrintIndent(file);
        fprintf(file, "FUNC\n");
        __expr_pretty_print_indent_level++;
        ExprASTPrint(file, expr->expr.call.func);
        __expr_pretty_print_indent_level--;
        ExprASTPrintIndent(file);
        fprintf(file, "ARG\n");
        __expr_pretty_print_indent_level++;
        ExprASTPrint(file, expr->expr.call.arg);
        __expr_pretty_print_indent_level--;
        __expr_pretty_print_indent_level--;
        break;
    case ExprKindAdd:
        ExprASTPrintIndent(file);
        fprintf(file, "ADD\n");
        __expr_pretty_print_indent_level++;
        ExprASTPrintIndent(file);
        fprintf(file, "LHS ");
        ExprASTPrint(file, expr->expr.add.lhs);
        ExprASTPrintIndent(file);
        fprintf(file, "RHS ");
        ExprASTPrint(file, expr->expr.add.rhs);
        __expr_pretty_print_indent_level--;
        break;
    case ExprKindSub:
        ExprASTPrintIndent(file);
        fprintf(file, "SUB\n");
        __expr_pretty_print_indent_level++;
        ExprASTPrintIndent(file);
        fprintf(file, "LHS ");
        ExprASTPrint(file, expr->expr.sub.lhs);
        ExprASTPrintIndent(file);
        fprintf(file, "RHS ");
        ExprASTPrint(file, expr->expr.sub.rhs);
        __expr_pretty_print_indent_level--;
        break;
    case ExprKindMul:
        ExprASTPrintIndent(file);
        fprintf(file, "MUL\n");
        __expr_pretty_print_indent_level++;
        ExprASTPrintIndent(file);
        fprintf(file, "LHS ");
        ExprASTPrint(file, expr->expr.mul.lhs);
        ExprASTPrintIndent(file);
        fprintf(file, "RHS ");
        ExprASTPrint(file, expr->expr.mul.rhs);
        __expr_pretty_print_indent_level--;
        break;
    case ExprKindDiv:
        ExprASTPrintIndent(file);
        fprintf(file, "DIV\n");
        __expr_pretty_print_indent_level++;
        ExprASTPrintIndent(file);
        fprintf(file, "LHS ");
        ExprASTPrint(file, expr->expr.div.lhs);
        ExprASTPrintIndent(file);
        fprintf(file, "RHS ");
        ExprASTPrint(file, expr->expr.div.rhs);
        __expr_pretty_print_indent_level--;
        break;
    case ExprKindMod:
        ExprASTPrintIndent(file);
        fprintf(file, "MOD\n");
        __expr_pretty_print_indent_level++;
        ExprASTPrintIndent(file);
        fprintf(file, "LHS ");
        ExprASTPrint(file, expr->expr.mod.lhs);
        ExprASTPrintIndent(file);
        fprintf(file, "RHS ");
        ExprASTPrint(file, expr->expr.mod.rhs);
        __expr_pretty_print_indent_level--;
        break;
    case ExprKindNeg:
        ExprASTPrintIndent(file);
        fprintf(file, "Neg\n");
        __expr_pretty_print_indent_level++;
        ExprASTPrintIndent(file);
        ExprASTPrint(file, expr->expr.neg.expr);
        __expr_pretty_print_indent_level--;
        break;
    default:
        ExprASTPrintIndent(file);
        fprintf(file, "ERROR");
        break;
    }
}

void ExprPrettyPrint(FILE* file, Expr* expr) {
    switch (expr->kind) {
    case ExprKindIdent:
        fprintf(file, "%s", expr->expr.ident.name);
        break;
    case ExprKindInteger:
        fprintf(file, "%ld", expr->expr.integer.value);
        break;
    case ExprKindString:
        fprintf(file, "\"%s\"", expr->expr.string.value);
        break;
    case ExprKindLabel:
        fprintf(file, "%s: ", expr->expr.label.label.name);
        ExprPrettyPrint(file, expr->expr.label.expr);
        break;
    case ExprKindDef:
        fprintf(file, "%s = ", expr->expr.def.ident.name);
        ExprPrettyPrint(file, expr->expr.def.expr);
        break;
    case ExprKindAssn:
        fprintf(file, "%s := ", expr->expr.assn.ident.name);
        ExprPrettyPrint(file, expr->expr.assn.expr);
        break;
    case ExprKindBlock:
        fprintf(file, "{\n");
        __expr_pretty_print_indent_level++;
        for (int i = 0; i < expr->expr.block.n; i++) {
            if (expr->expr.block.exprs[i]->kind != ExprKindTuple &&
                expr->expr.block.exprs[i]->expr.tuple.n != 0) {
                for (int j = 0; j < __expr_pretty_print_indent_level; j++) {
                    fprintf(file, "    ");
                }
                ExprPrettyPrint(file, expr->expr.block.exprs[i]);
                if (i != expr->expr.block.n - 1) {
                    fprintf(file, ";");
                }
                fprintf(file, "\n");
            }
        }
        __expr_pretty_print_indent_level--;
        for (int j = 0; j < __expr_pretty_print_indent_level; j++) {
            fprintf(file, "    ");
        }
        fprintf(file, "}");
        break;
    case ExprKindTuple:
        if (expr->expr.tuple.n == 0) {
            fprintf(file, "()");
        } else {
            for (int i = 0; i < expr->expr.tuple.n; i++) {
                ExprPrettyPrint(file, expr->expr.tuple.exprs[i]);
                if (i != expr->expr.tuple.n - 1) {
                    fprintf(file, ", ");
                }
            }
        }
        break;
    case ExprKindSup:
        for (int i = 0; i < expr->expr.sup.n; i++) {
            ExprPrettyPrint(file, expr->expr.sup.exprs[i]);
            if (i != expr->expr.sup.n - 1) {
                fprintf(file, " | ");
            }
        }
        break;
    case ExprKindFunc:
        ExprPrettyPrint(file, expr->expr.func.arg);
        fprintf(file, " -> ");
        ExprPrettyPrint(file, expr->expr.func.body);
        break;
    case ExprKindCall:
        ExprPrettyPrint(file, expr->expr.call.func);
        fprintf(file, "(");
        ExprPrettyPrint(file, expr->expr.call.arg);
        fprintf(file, ")");
        break;
    case ExprKindAdd:
        ExprPrettyPrint(file, expr->expr.add.lhs);
        fprintf(file, " + ");
        ExprPrettyPrint(file, expr->expr.add.rhs);
        break;
    case ExprKindSub:
        ExprPrettyPrint(file, expr->expr.sub.lhs);
        fprintf(file, " - ");
        ExprPrettyPrint(file, expr->expr.sub.rhs);
        break;
    case ExprKindMul:
        ExprPrettyPrint(file, expr->expr.mul.lhs);
        fprintf(file, " * ");
        ExprPrettyPrint(file, expr->expr.mul.rhs);
        break;
    case ExprKindDiv:
        ExprPrettyPrint(file, expr->expr.div.lhs);
        fprintf(file, " / ");
        ExprPrettyPrint(file, expr->expr.div.rhs);
        break;
    case ExprKindMod:
        ExprPrettyPrint(file, expr->expr.mod.lhs);
        fprintf(file, " %% ");
        ExprPrettyPrint(file, expr->expr.mod.rhs);
        break;
    case ExprKindNeg:
        fprintf(file, "-");
        ExprPrettyPrint(file, expr->expr.neg.expr);
        break;
    default:
        fprintf(file, "<ERROR>");
    }
}
