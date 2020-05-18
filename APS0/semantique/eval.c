#include <stdio.h>
#include <string.h>
#include "eval.h"


Env new_env(){
    Env env = (Env)malloc(sizeof(struct _env));
    env->cap=ENVSIZE;
    env->size=0;
    return env;
}

Env copy_env(Env env){
    Env copy = (Env)malloc(sizeof(struct _env));
    memcpy(copy, env, sizeof(struct _env));
    return copy;
}

void print_env(Env env){
    printf("<");
    for(int i=0; i<env->size; i++){
        if(env->vals[i]){
            if(env->vals[i]->tag==V_INT) printf("(%s, %d)", env->idents[i], get_num(env->vals[i]));
            else if(env->vals[i]->tag==V_FUN) printf("(%s, fun)", env->idents[i]);
            else printf("(%s, funrec)", env->idents[i]);
        }else{
            printf("(%s, NULL)", env->idents[i]);
        }
    }
    printf(">\n");
}


void add_env(Env env, char *ident, Value v){
    if(env->size == env->cap){
        perror("stack overflow"); exit(0);
    }
    env->idents[env->size]=ident;
    env->vals[env->size++]=v;
}

Value get_env(Env env, char *id){
    if(env->size==0) return NULL;
    for(int i=env->size-1; i>=0; i--){
        if(!strcmp(env->idents[i], id)) return env->vals[i];
    }
    return NULL;
}

void pop_env(Env env, int size){
    if(size < 0) return;
    for(int i=0; i<size; i++){
        env->idents[env->size] = NULL;
        free_value(env->vals[env->size]);
        env->vals[env->size--];
    }
}

void free_value(Value v){
    if (v==NULL) return;
    switch (v->tag){
    case V_INT:
        free(v);
        break;
    case V_FUN:
        free(v->content.fun->args);
        free(v->content.fun->body);
        free(v->content.fun);
        free(v);
        break;
    case V_FUNREC:
        free(v->content.funrec->fun->args);
        free(v->content.funrec->fun->body);
        free(v->content.funrec->fun);
        free(v);
        break;
    default:
        break;
    }
}

Value new_num(int v){
    Value value = (Value)malloc(sizeof(struct _value));
    value->tag=V_INT;
    value->content.val=v;
    return value;
}

int get_num(Value v){
    if(v->tag!=V_INT) exit(0);
    return v->content.val;
}

Value new_fun(int argc, char**args, Expr body, Env env){
    Value value = (Value)malloc(sizeof(struct _value));
    value->tag=V_FUN;
    Fun fun = (Fun)malloc(sizeof(struct _fun));
    fun->argc=argc;
    fun->args=args;
    fun->body=body;
    fun->env=copy_env(env);
    value->content.fun=fun;
    return value;
}

int nb_args(Args args){
    int res = 0;
    while(args){
        args=args->next;
        res++;
    }
    return res;
}

char ** get_args(Args args){
    int nb = nb_args(args);
    char **res = (char **)malloc(sizeof(char *) * nb);
    char **tail = res;
    for(int i=0; i<nb; i++){
        *(tail+i) = args->arg->ident;
        args=args->next;
    }
    return res;
}

void print_args(char** args, int argc){
    printf("args=");
    for(int i=0; i<argc;i++){
        printf("%s ", *(args+i));
    }
    printf("\n");
}

Value app_fun(Exprs es, Fun fun, Env env){
    Value val;
    for(int i=0; i<env->size; i++){
        val = get_env(fun->env, env->idents[i]);
        if(val) continue;
        else add_env(fun->env, env->idents[i], env->vals[i]);
    }
    for(int i=0; i<fun->argc; i++){
        val = evalExpr(es->head, env);
        es=es->next;
        add_env(fun->env, *(fun->args+i), val);
    }
    Value res=evalExpr(fun->body, fun->env);
    if(res->tag==V_INT) pop_env(fun->env, fun->argc);
    return res;
}


Fun get_fun(Value v){
    if(v->tag!=V_FUN) exit(0);
    return v->content.fun;
}

Value new_funrec(char *id, Fun fun){
    Value value = (Value)malloc(sizeof(struct _value));
    value->tag=V_FUNREC;
    value->content.funrec=(Funrec)malloc(sizeof(struct _funrec));
    value->content.funrec->id=id;
    value->content.funrec->fun=fun;
    return value;
}

Funrec get_funrec(Value v){
    if(v->tag!=V_FUNREC) exit(0);
    return v->content.funrec;
}

Value evalBinOp(Oprim op, Exprs es, Env env) {
    Value v1 = evalExpr(es->head, env);
    es = es->next;
    Value v2 = evalExpr(es->head, env);
    int n1 = get_num(v1), n2= get_num(v2), res=0;
    switch(op) {
    case Add : res = n1+n2; break;
    case Sub : res = n1-n2; break;
    case Mul : res = n1*n2; break;
    case Div : res = n1/n2; break;
    case And : res = n1 && n2; break;
    case Or : res = n1 || n2; break;
    case Lt : res = n1 < n2; break;
    case Gt : res = n1 > n2; break;
    case Eq : res = n1 == n2; break;
    default:
        perror("op is not defined"); exit(0);
    }
    return new_num(res);
}



Value evalExpr(Expr e, Env env) {
    Value res, cond; 
    Fun fun;
    int size, argc;
    char **args;
    Env newenv=NULL;
    switch(e->tag) {
    case ASTNum : return new_num(e->content.num); 
    case ASTId : 
        res=get_env(env, e->content.id);
        if(res == NULL) {
            printf("%s is not defined\n", e->content.id); exit(0);
        }
        return res;
    case ASTBool:
        if(e->content.cbool.val==c_true) return new_num(1);
        return new_num(0);
    case ASTPrim: 
        return evalBinOp(e->content.prim.op, e->content.prim.opans, env);
    case ASTIf:
        cond=evalExpr(e->content.If.condition, env);
        if(get_num(cond)) return evalExpr(e->content.If.prog, env);
        return evalExpr(e->content.If.alter, env);
    case ASTAppfun:
        res = evalExpr(e->content.es->head, env);
        if(res->tag==V_FUN){
            fun=res->content.fun;
            return app_fun(e->content.es->next, fun, env);
        }else if(res->tag==V_FUNREC){
            fun=res->content.funrec->fun;
            return app_fun(e->content.es->next, fun, env);
        }
        break;
    case ASTLambda:
        argc = nb_args(e->content.lambda.args);
        args = get_args(e->content.lambda.args);
        res = new_fun(argc, args, e->content.lambda.e, env);
        return res;
    default:
        break;
    }
    return new_num(-1);
}

void evalDec(Dec dec, Env env){
    Value res; Args args;
    int argc; char **argstr;
    switch (dec->tag){
    case DEC_CONS:
        res = evalExpr(dec->content._const.e, env);
        add_env(env, dec->id, res);
        break;
    case DEC_FUN:
        args = dec->content._fun.args;
        argc = nb_args(args);
        argstr = get_args(args);
        res = new_fun(argc, argstr, dec->content._fun.e, env);
        add_env(env, dec->id, res);
        break;
    case DEC_FUNREC:
        args = dec->content._fun.args;
        argc = nb_args(args);
        argstr = get_args(args);
        res = new_fun(argc, argstr, dec->content._fun.e, env);
        res = new_funrec(dec->id, res->content.fun);
        add_env(env, dec->id, res);
        break;
    default:
        break;
    }
}

void evalCmd(Cmd cmd, Env env) {
    int size;
    switch (cmd->tag) {
        case CMD_STAT:
            printf("%d\n", get_num(evalExpr(cmd->content.stat->content.e, env))); 
            break;
        case CMD_DEC: evalDec(cmd->content.dec, env);
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
    // print_env(env);
}


