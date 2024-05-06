#include "expr.h"

#include <stdio.h>

int __expr_pretty_print_indent_level = 0;

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
    }
}
