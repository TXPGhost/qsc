#include "token.h"
#include "log.h"

const char* const TokenGetKindString(Token token) {
    switch (token) {
    case TokNone:
        return "None";
    case TokIdent:
        return "Ident";
    case TokEquals:
        return "Equals";
    case TokSetEquals:
        return "SetEquals";
    case TokEqualsEquals:
        return "EqualsEquals";
    case TokSemicolon:
        return "Semicolon";
    case TokNumber:
        return "Number";
    case TokComma:
        return "Comma";
    case TokLParen:
        return "LParen";
    case TokRParen:
        return "RParen";
    case TokLAngle:
        return "LAngle";
    case TokRAngle:
        return "RAngle";
    case TokLCurly:
        return "LCurly";
    case TokRCurly:
        return "RCurly";
    case TokLBracket:
        return "LBracket";
    case TokRBracket:
        return "RBracket";
    case TokDot:
        return "Dot";
    case TokColon:
        return "Colon";
    case TokBar:
        return "Bar";
    case TokArrow:
        return "Arrow";
    case TokString:
        return "String";
    case TokChar:
        return "Char";
    case TokAdd:
        return "Add";
    case TokSub:
        return "Sub";
    case TokMul:
        return "Mul";
    case TokDiv:
        return "Div";
    case TokMod:
        return "Mod";
    default:
        ERROR("unimplemented TokenGetKindString for Token %d", token);
        return "<UNIMPLEMENTED>";
    }
};
