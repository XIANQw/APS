#ifndef AST_H
#define AST_H

#include <stdlib.h>
typedef enum{
    CMD_DEC, CMD_STAT
}TagCmd;
typedef enum{
    DEC_CONS, DEC_FUN, DEC_FUNREC, DEC_VAR, DEC_PROC, DEC_PROCREC
}TagDec;
typedef enum{
    STAT_ECHO, STAT_SET, STAT_IF, STAT_WHILE, STAT_CALL
}TagStat;
typedef enum{
    ASTNum, ASTId, ASTBool, ASTNot, ASTPrim, 
    ASTBloc, ASTIf, ASTLambda
}TagExpr;
typedef enum{
    Add, Sub, Mul, Div, Eq, Lt, Gt, Or, And, Not
}Oprim;

typedef enum{
    T_INT, T_BOOL, T_VOID
}Tprim;

typedef enum{
    c_true, c_false
}cbool;

typedef struct _prog * Prog;
typedef struct _exprs * Exprs;
typedef struct _expr * Expr;
typedef struct _dec * Dec;
typedef struct _stat * Stat;
typedef struct _cmd * Cmd;
typedef struct _cmds * Cmds;
typedef struct _type * Type;
typedef struct _types * Types;
typedef struct _arg * Arg;
typedef struct _args * Args;


struct _prog{
    Cmds content;
};

struct _dec{
    TagDec tag;
    char *id;
    union{ 
        struct{
            Type type;
            Expr e;
        }_const;
        struct{
            Type type;
        }_var;
        struct {
            Type type;
            Args args;
            Expr e;
        }_fun;
        struct{
            Args args;
            Cmds cmds;
        }_proc;
    }content;
};

struct  _stat{
    TagStat tag;
    union {
        Expr e;
        struct{
            char *id;
            Expr e;
        }_set;
        struct{
            Expr cond;
            Cmds res;
            Cmds alter;
        }_if;
        struct{
            Expr cond;
            Cmds block;
        }_while;
        struct{
            char *id;
            Exprs es;
        }_call;
    }content;  
};


struct _cmd{
    TagCmd tag;
    union{
        Dec dec;
        Stat stat;
    }content;
};

struct _cmds{
    Cmd head;
    Cmds next;
};

struct _type{
    int flag;
    union{
        Tprim t;
        struct{
            Types types;
            Type type;
        }t_func;
    }content;
};

struct _types{
    Type head;
    Types next;
};

struct _arg {
    char * ident;
    Type type;
};

struct _args {
    Arg arg;
    Args next;
};

struct _expr{
    TagExpr tag;
    union{
        int num;
        char *id;
        Expr not;
        struct{
            cbool val;
        }cbool;
        struct{
            Oprim op;
            Exprs opans;
        }prim;
        struct{
            Expr condition;
            Expr prog;
            Expr alter;  
        }If;
        struct{
            Args args;
            Expr e;
        }lambda;
        Exprs es;
    }content;
};

struct _exprs{
    Expr head;
    Exprs next;
};

// Prog
Prog newASTProg(Cmds cmds);
// Cmds
Cmds appendCmds(Cmd cmd, Cmds cmds);
Cmd newASTCmd(Stat stat, Dec dec, TagCmd tag);
// Stat
Stat newASTStatEcho(Expr e);
Stat newASTStatSet(char *id, Expr e);
Stat newASTStatIf(Expr cond, Prog prog, Prog alter);
Stat newASTStatWhile(Expr cond, Prog block);
Stat newASTStatCall(char *id, Exprs es);
// Dec
Dec newASTDec(char * id, Type t, Args args, Expr e, Prog block, TagDec tag);
// Types
Type newASTType1(Tprim t);
Type newASTType2(Types types, Type type);
Types appendTypes(Type type, Types tpyes);
// Args
Arg newASTArg(char * id, Type type);
Args appendArgs(Arg arg, Args args);
// Expr
Expr newASTNum(int num);
Expr newASTId(char * id);
Expr newASTBool(cbool val);
Expr newASTNot(Expr e);
Expr newASTPrim(Oprim op, Exprs es);
Expr newASTIf(Expr cond, Expr res, Expr alter);
Expr newASTLambda(Args args, Expr e);
Expr newASTBloc(Exprs es);
Exprs appendExprs(Expr e, Exprs es);

#define mallocProg (Prog)malloc(sizeof(struct _prog))
#define mallocCmds (Cmds)malloc(sizeof(struct _cmds))
#define mallocCmd (Cmd)malloc(sizeof(struct _cmd))
#define mallocStat (Stat)malloc(sizeof(struct _stat))
#define mallocDec (Dec)malloc(sizeof(struct _dec))
#define mallocTypes (Types)malloc(sizeof(struct _types))
#define mallocType (Type)malloc(sizeof(struct _type))
#define mallocArgs (Args)malloc(sizeof(struct _args))
#define mallocArg (Arg)malloc(sizeof(struct _arg))
#define mallocExprs (Exprs)malloc(sizeof(struct _exprs))
#define mallocExpr (Expr)malloc(sizeof(struct _expr))



#endif