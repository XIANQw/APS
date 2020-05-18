#include <stdio.h>
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
    res->content.cbool.val = val;
    return res;
}

Expr newASTNot(Expr e){
    Expr res = mallocExpr;
    res->tag = ASTNot;
    res->content.not=e;
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

Cmd newASTCmd(Stat stat, Dec dec, TagCmd tag) {
    Cmd res = mallocCmd;
    res->tag = tag;
    if(tag==CMD_DEC) res->content.dec = dec;
    else if(tag==CMD_STAT) res->content.stat =stat;
    else{
        printf("Tag undifined\n"); exit(0);
    }
    return res;
}

Stat newASTStatEcho(Expr e){
    Stat res = mallocStat;
    res->tag = STAT_ECHO;
    res->content.e = e;
    return res;
}
Stat newASTStatSet(char *id, Expr e){
    Stat res = mallocStat;
    res->tag = STAT_SET;
    res->content._set.id = id;
    res->content._set.e = e;
    return res;
}
Stat newASTStatIf(Expr cond, Prog block, Prog alter){
    Stat res = mallocStat;
    res->tag = STAT_IF;
    res->content._if.cond=cond;
    res->content._if.res=block->content;
    res->content._if.alter=alter->content;
    return res;
}
Stat newASTStatWhile(Expr cond, Prog block){
    Stat res = mallocStat;
    res->tag = STAT_WHILE;
    res->content._while.cond=cond;
    res->content._while.block=block->content;
    return res;
}
Stat newASTStatCall(char *id, Exprs es){
    Stat res=mallocStat;
    res->tag=STAT_CALL;
    res->content._call.id=id;
    res->content._call.es=es;
    return res;
}

Dec newASTDec(char * id, Type t, Args args, Expr e, Prog prog, TagDec tag){
    Dec res=mallocDec;
    res->id=id;
    res->tag=tag;
    if(tag==DEC_CONS){
        res->content._const.type=t;
        res->content._const.e=e;
    }else if(tag==DEC_FUN||tag==DEC_FUNREC){
        res->content._fun.args=args;
        res->content._fun.type=t;
        res->content._fun.e=e;
    }else if(tag==DEC_VAR){
        res->content._var.type=t;
    }else if(tag==DEC_PROC || tag==DEC_PROCREC){
        res->content._proc.args=args;
        res->content._proc.cmds=prog->content;
    }
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
    res->tag = ASTAppfun;
    res->content.es = es;
    return res;
}