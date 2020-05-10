#include "prologTerm.h"
#include <stdio.h>

void printOp(Oprim op){
    switch(op){
        case Add: printf("add"); break;
        case Sub: printf("sub"); break;
        case Div: printf("div"); break;
        case Mul: printf("mul"); break;
        case Or: printf("or"); break;
        case And: printf("and"); break;        
        case Eq: printf("eq"); break;
        case Lt: printf("less"); break;
        case Gt: printf("gt"); break;        
    }
}

void printNum(int n){
    printf("%d", n);
}
void printId(char * id){
    printf("%s", id);
}

void printBool(cbool val) {
    switch (val) {
        case c_true : printf("true"); break;
        case c_false: printf("false"); break;
    }
}

void printPrim(Expr e){
    printOp(e->content.prim.op);
    printf("(");
    printExprs(e->content.prim.opans);
    printf(")");
}

void printIf(Expr e){
    printf("if(");
    printExpr(e->content.If.condition);
    printf(",");
    printExpr(e->content.If.prog);
    printf(",");
    printExpr(e->content.If.alter);
    printf(")");
}

void printLambda(Expr e){
    printf("lambda(");
    printArgs(e->content.lambda.args);
    printf(",");
    printExpr(e->content.lambda.e);
    printf(")");
}

void printBloc(Expr e) {
    printf("astExprs(");
    printExprs(e->content.bloc);
    printf(")");
}

void printExpr(Expr e){
    switch(e->tag) {
        case ASTNum : printNum(e->content.num); break;
        case ASTId : printId(e->content.id); break;
        case ASTBool : printBool(e->content.bool.val); break;
        case ASTPrim : printPrim(e); break;
        case ASTIf: printIf(e); break;
        case ASTLambda: printLambda(e); break;
        case ASTBloc: printBloc(e); break;
    }
}

void printExprs(Exprs es){
    if(!es) return ;
    while(es->next){
        printExpr(es->head);
        printf(",");
        es = es->next;
    }
    printExpr(es->head);
}

void printType(Type t){
    switch (t->flag){
        case 1:{
            switch (t->content.t) {
                case T_INT:printf("int");break;
                case T_BOOL: printf("bool");break;
            }
            break;
        }
        case 2:{
            printf("(");
            printTypes(t->content.t_func.types);
            printf("->");
            printType(t->content.t_func.type);
            printf(")");
            break;
        }
    }
}

void printTypes(Types ts){
    if(!ts) return ;
    while(ts->next){
        printType(ts->head);
        printf("*");
        ts = ts->next;
    }
    printType(ts->head);
}

void printDec(Dec dec){
    switch (dec->tag) {
        case DEC_CONS:
            printf("const(");
            printId(dec->id);
            printf(",");
            printType(dec->type);
            printf(",");
            printExpr(dec->e);
            printf(")");
            break;
        case DEC_FUN: 
            printf("fun("); 
            printId(dec->id);printf(",");
            printType(dec->type);printf(",");
            printArgs(dec->args);printf(",");
            printExpr(dec->e);
            printf(")");
            break;
        case DEC_FUNREC : 
            printf("funrec("); 
            printId(dec->id);
            printf(",");
            printType(dec->type);
            printf(",");
            printArgs(dec->args);
            printf(",");
            printExpr(dec->e);
            printf(")");
            break;
    }
}

void printArg(Arg arg) {
    if(!arg) return;
    printf("(");
    printId(arg->ident);
    printf(":");
    printType(arg->type);
    printf(")");
}

void printArgs(Args args){
    if(!args) return ;
    printf("[");
    while(args->next){
        printArg(args->arg);
        printf(",");
        args = args->next;
    }
    printArg(args->arg);
    printf("]");
}

void printCmd(Cmd cmd) {
    switch(cmd->tag) {
        case ASTStat : 
        printf("echo(");
        printExpr(cmd->content.stat.content); 
        printf(")");
        break;
        case ASTDec : printDec(cmd->content.dec); break;
    }
}

void printCmds(Cmds cmds){
    if(!cmds) return ;
    while(cmds->next){
        printCmd(cmds->head);
        printf(";");
        cmds = cmds->next;
    }
    printCmd(cmds->head);
}


void printProg(Prog prog){
    printf("program([");
    printCmds(prog->content);
    printf("]).");
}