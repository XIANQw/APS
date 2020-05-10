#include <stdio.h>
#include <string.h>
#include "eval.h"


Env new_env(){
    Env env = (Env)malloc(sizeof(_env));
    env->cap=ENVSIZE;
    env->size=0;
    env->funsize=0;
    return env;
}

void add_env(Env env, char *ident, int value){
    if(env->size == env->cap){
        perror("stack overflow"); exit(0);
    }
    env->idents[env->size]=ident;
    env->values[env->size++]=value;
}

void add_fun_env(Env env, char *ident, Dec fun){
    if(env->funsize==env->cap){
        perror("stack overflow"); exit(0);
    }
    env->funident[env->funsize]=ident;
    env->func[env->funsize++] = fun;
}

int get_env(Env env, char *ident){
    for(int i=0; i< env->size; i++){
        if(!strcmp(env->idents[i], ident)) return env->values[i];
    }
    return __INT_MAX__;
}

Dec get_fun(Env env, char *ident){
    for(int i=0; i < env->funsize; i++){
        if(!strcmp(env->funident[i], ident)) return env->func[i];
    }
    return NULL;
}

int evalBinOp(Oprim op, Exprs es, Env env) {
    int n1 = evalExpr(es->head, env);
    es = es->next;
    int n2 = evalExpr(es->head, env);
    int res;
    switch(op) {
    case Add : return n1+n2; 
    case Sub : return n1-n2; 
    case Mul : return n1*n2; 
    case Div : return n1/n2; 
    }
}

cbool evalBOprim(Oprim op, Exprs es, Env env){
    int n1 = evalExpr(es->head, env);
    es = es->next;
    int n2 = evalExpr(es->head, env);
    int res = 0;
    switch(op) {
    case And : res = n1 && n2; break; 
    case Or : res = n1 || n2; break;
    case Lt : res = n1 < n2; break;
    case Gt : res = n1 > n2; break;
    case Eq : res = n1 == n2; break;
    }
    if(res) return c_true;
    return c_false;
}


int evalExpr(Expr e, Env env) {
    int res, cond;
    switch(e->tag) {
    case ASTNum : return e->content.num; 
    case ASTId : 
        res=get_env(env, e->content.id);
        if(res==__INT_MAX__){
            perror("varibale is not defined"); exit(0);
        }
        return res;
    case ASTBool:
        if(e->content.bool.val==c_true) return 1;
        else return 0;
    case ASTPrim : return evalBinOp(e->content.prim.op, e->content.prim.opans, env);
    case ASTIf:
        cond=evalExpr(e->content.If.condition, env);
        if(cond) return evalExpr(e->content.If.prog, env);
        else return evalExpr(e->content.If.alter, env);
    case ASTBloc:
        return 0;
        break;
    default:
        return 0;
        break;
    }
    return 0;
}

void evalDec(Dec dec, Env env){
    int res;
    switch (dec->tag){
    case DEC_CONS:
        res = evalExpr(dec->e, env);
        add_env(env, dec->id, res);
        break;
    case DEC_FUN:
        printf("dec fun\n");
        exit(0);
    case DEC_FUNREC:
        printf("fun rec\n");
        exit(0);
    default:
        break;
    }
}

void evalCmd(Cmd cmd, Env env) {
    switch (cmd->tag) {
        case ASTStat: printf("%d\n",evalExpr(cmd->content.stat.content, env)); break;
        case ASTDec: evalDec(cmd->content.dec, env);
    }
}

void evalCmds(Cmds cmds, Env env){
    if(!cmds) return ;
    while(cmds->next){
        evalCmd(cmds->head, env);
        cmds = cmds->next;
    }
    evalCmd(cmds->head, env);
}

void evalProg(Prog p) {
    Env env = new_env();
    evalCmds(p->content, env);
}

