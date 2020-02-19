#include <stdlib.h>

typedef enum{
    ASTNum, ASTId, ASTBool, ASTPrim, ASTBloc, ASTStat, ASTDec, ASTFun
}Tag;

typedef enum{
    Eq, Lt, Gt, Add, Sub, Mul, Div, Or, And
}Oprim;

typedef enum{
    T_INT, T_BOOL
}Type;

typedef enum{
    c_true, c_false
}cbool;


typedef struct _exprs * Exprs;
typedef struct _expr * Expr;
typedef struct _cmd * Cmd;
typedef struct _cmds * Cmds;
typedef struct _prog * Prog;

struct _prog{
    Cmds content;
};

struct _cmd{
    Tag tag;
    union{
        struct {
            char * id;
            Type type;
            Expr e;
        }dec;
        struct {
            Expr content;
        }stat;
    }content;
};

struct _cmds{
    Cmd head;
    Cmds next;
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
            Oprim op;
            Exprs opans;
        }prim;
    }content;
};

struct _exprs{
    Expr head;
    Exprs next;
};


Expr newASTNum(int num);
Expr newASTId(char * id);
Expr newASTBool(cbool val);
Expr newASTPrim(Oprim, Exprs es);
Exprs appendExprs(Expr e, Exprs es);
Cmds appendCmds(Cmd cmd, Cmds cmds);
Cmd newASTStat(Expr e);
Cmd newASTDec(char * id, Type t, Expr e);


#define mallocExpr (Expr)malloc(sizeof(struct _expr))
#define mallocExprs (Exprs)malloc(sizeof(struct _exprs))
#define mallocCmd (Cmd)malloc(sizeof(struct _cmd))
#define mallocCmds (Cmds)malloc(sizeof(struct _cmds))
