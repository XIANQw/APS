#include "ast.h"

Prog newASTProg(Cmds cmds){
    Prog res = mallocProg;
    res->content =cmds;
    return res;
}


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
    if(op < 4) res->content.prim.tag = ASTPrim;
    else if(op < 7) res->content.prim.tag = ASTBprim;
    else res->content.prim.tag = ASTRprim;
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

Cmd newASTCmdDec(Dec dec) {
    Cmd res = mallocCmd;
    res->tag = ASTDec;
    res->content.dec = dec;
    return res;
}

Dec newASTDec(char * id, Type t, Args args, Expr e, TagDec tag){
    Dec res = mallocDec;
    res->tag = tag;
    res->id = id;
    res->type = t;
    res->args = args;
    res->e = e;
    return res;
}


Cmds appendCmds(Cmd cmd, Cmds cmds) {
    Cmds res = mallocCmds;
    res->head = cmd;
    res->next = cmds;
    return res;
}

Type newASTType1(Tprim t){
    Type res = mallocType;
    res->flag = 1;
    res->content.t = t;
    return res;
}

Type newASTType2(Types t1, Type t2){
    Type res = mallocType;
    res->flag = 2;
    res->content.t_func.types = t1;
    res->content.t_func.type = t2;
    return res;
}

Types appendTypes(Type t, Types ts){
    Types res = mallocTypes;
    res->head = t;
    res->next = ts;
    return res; 
}

Arg newASTArg(char * id, Type type){
    Arg res = mallocArg;
    res->ident = id;
    res->type = type;
    return res;
}

Args appendArgs(Arg arg, Args args){
    Args res = mallocArgs;
    res->arg = arg;
    res->next = args;
    return res;
}

Expr newASTIf(Expr cond, Expr prog, Expr alter) {
    Expr res = mallocExpr;
    res->tag = ASTIf;
    res->content.If.condition = cond;
    res->content.If.prog = prog;
    res->content.If.alter = alter;
    return res;
}

Expr newASTLambda(Args args, Expr e) {
    Expr res = mallocExpr;
    res->tag = ASTLambda;
    res->content.lambda.args = args;
    res->content.lambda.e = e;
    return res;
}

Expr newASTBloc(Exprs es){
    Expr res = mallocExpr;
    res->tag = ASTBloc;
    res->content.bloc = es;
    return res;
}