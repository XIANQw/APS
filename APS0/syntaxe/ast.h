#include <stdlib.h>

typedef enum{
    ASTNum, ASTId, ASTBool, ASTPrim, 
    ASTBprim, ASTRprim, ASTBloc, ASTStat, ASTDec, 
    ASTIf, ASTLambda 
}Tag;
typedef enum{
    DEC_CONS, DEC_FUN, DEC_FUNREC
}TagDec;

typedef enum{
    Add, Sub, Mul, Div, Eq, Lt, Gt, Or, And
}Oprim;

typedef enum{
    T_INT, T_BOOL
}Tprim;

typedef enum{
    c_true, c_false
}cbool;

typedef struct _prog * Prog;
typedef struct _exprs * Exprs;
typedef struct _expr * Expr;
typedef struct _dec * Dec;
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
    char * id;
    Type type;
    Expr e;
    Args args;    
};


struct _cmd{
    Tag tag;
    union{
        Dec dec;
        struct {
            Expr content;
        }stat;
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
    Tag tag;
    union{
        int num;
        char *id;
        struct{
            cbool val;
        }bool;
        struct{
            Tag tag;
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
        Exprs bloc;
    }content;
};

struct _exprs{
    Expr head;
    Exprs next;
};

Prog newASTProg(Cmds cmds);
Expr newASTNum(int num);
Expr newASTId(char * id);
Expr newASTBool(cbool val);
Expr newASTPrim(Oprim op, Exprs es);
Expr newASTIf(Expr cond, Expr res, Expr alter);
Expr newASTLambda(Args args, Expr e);
Expr newASTBloc(Exprs es);
Exprs appendExprs(Expr e, Exprs es);
Cmds appendCmds(Cmd cmd, Cmds cmds);
Cmd newASTStat(Expr e);
Cmd newASTCmdDec(Dec dec);
Dec newASTDec(char * id, Type t, Args args, Expr e, TagDec tag);
Type newASTType1(Tprim t);
Type newASTType2(Types types, Type type);
Types appendTypes(Type type, Types tpyes);
Arg newASTArg(char * id, Type type);
Args appendArgs(Arg arg, Args args);

#define mallocProg (Prog)malloc(sizeof(struct _prog))
#define mallocExpr (Expr)malloc(sizeof(struct _expr))
#define mallocExprs (Exprs)malloc(sizeof(struct _exprs))
#define mallocCmd (Cmd)malloc(sizeof(struct _cmd))
#define mallocCmds (Cmds)malloc(sizeof(struct _cmds))
#define mallocDec (Dec)malloc(sizeof(struct _dec))
#define mallocType (Type)malloc(sizeof(struct _type))
#define mallocTypes (Types)malloc(sizeof(struct _types))
#define mallocArg (Arg)malloc(sizeof(struct _arg))
#define mallocArgs (Args)malloc(sizeof(struct _args))