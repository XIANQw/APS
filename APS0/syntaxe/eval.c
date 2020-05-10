#include <stdio.h>
#include <string.h>
#include "ast.h"
#include "eval.h"


Env new_env(){
    Env env = (Env)malloc(sizeof(_env));
    env->capacity=1024;
    env->size=0;
    return env;
}

void add_env(Env env, char *ident, int value){
    if(env->size == env->capacity){
        perror("stack overflow"); exit(0);
    }
    env->idents[env->size]=ident;
    env->values[env->size]=value;
    env->size++;
}

int get_env(Env env, char *ident){
    for(int i=0; i< env->size; i++){
        if(!strcmp(env->idents[i], ident)) return env->values[i];
    }
    return -1;
}


int evalBinOp(Oprim op, Exprs es, Env env) {
    int n1 = evalExpr(es->head, env);
    es = es->next;
    int n2 = evalExpr(es->head, env);
    switch(op) {
    case Add : return n1+n2; 
    case Sub : return n1-n2; 
    case Mul : return n1*n2; 
    case Div : return n1/n2; 
    }
}

int evalExpr(Expr e, Env env) {
    switch(tagOf(e)) {
    case ASTNum : return e->content.num; 
    case ASTPrim : return evalBinOp(e->content.prim.op, e->content.prim.opans, env);
    }
}

void evalCmd(Cmd cmd, Env env) {
    switch (cmd->tag) {
        case ASTStat : printf("%d\n",evalExpr(cmd->content.stat.content, env));
        case ASTDec : 
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
    printf("\n");
}

