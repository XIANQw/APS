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
        case Eq: printf("equals"); break;
        case Lt: printf("less"); break;
        case Gt: printf("greater"); break;        
    }
}

void printNum(int n){
    printf("num(%d)", n);
}
void printId(char * id){
    printf("var(%s)", id);
}

void printBool(cbool val) {
    switch (val) {
        case c_true : printf("true"); break;
        case c_false: printf("false"); break;
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
    switch (t){
        case T_INT:{
            printf("int");
            break;
        }
        case T_BOOL:{
            printf("bool");
            break;
        }
    }
}

void printExpr(Expr e){
    switch(e->tag) {
        case ASTNum : printNum(e->content.num); break;
        case ASTId : printId(e->content.id); break;
        case ASTBool : printBool(e->content.bool.val); break;
        case ASTPrim : {
            printOp(e->content.prim.op);
            printf("([");
            printExprs(e->content.prim.opans);
            printf("])");
            break;
        }
    }
}


void printStat(Cmd stat){
    printf("Echo: ");
    printExpr(stat->content.stat.content);
}

void printDec(Cmd dec){
    printId(dec->content.dec.id);
    printf(": ");
    printType(dec->content.dec.type);
    printf(" = ");
    printExpr(dec->content.dec.e);
}


void printCmd(Cmd cmd) {
    switch(cmd->tag) {
        case ASTStat : printStat(cmd); break;
        case ASTDec : printDec(cmd); break;
    }
}


void printCmds(Cmds cmds){
    if(!cmds) return ;
    while(cmds->next){
        printCmd(cmds->head);
        printf(";\n");
        cmds = cmds->next;
    }
    printCmd(cmds->head);
}

