#include "ast.h"

Expr newASTNum(int num){
    Expr res = mallocExpr;
    res->tag = ASTNum;
    res->content.num = num;
    return res;
}

Expr newASTId(char * id){
    Expr res = mallocExpr;
    res->tag = ASTId;
    res->content.id = id;
    return res;
}

Expr newASTBool(cbool val){
    Expr res = mallocExpr;
    res->tag = ASTBool;
    res->content.bool.val = val;
    return res;
}

Expr newASTPrim(Oprim op, Exprs es){
    Expr res = mallocExpr;
    res->tag = ASTPrim;
    res->content.prim.op = op;
    res->content.prim.opans = es;
    return res;
}

Exprs appendExprs(Expr e, Exprs es){
    Exprs res = mallocExprs;
    res->head = e;
    res->next = es;
    return res;
}

Cmd newASTStat(Expr e){
    Cmd res = mallocCmd;
    res->tag = ASTStat;
    res->content.stat.content = e;
    return res;
}

Cmd newASTDec(char * id, Type t, Expr e){
    Cmd res = mallocCmd;
    res->tag = ASTDec;
    res->content.dec.id = id;
    res->content.dec.type = t;
    res->content.dec.e = e;
    return res;
}

Cmds appendCmds(Cmd cmd, Cmds cmds) {
    Cmds res = mallocCmds;
    res->head = cmd;
    res->next = cmds;
    return res;
}



