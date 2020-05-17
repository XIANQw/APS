#include "ast.h"

typedef enum {
    V_INT, V_FUN, V_FUNREC, V_ADDR, V_PROC, V_PROCREC, V_VOID
}tag_value;

typedef struct _value * Value;
typedef struct _fun * Fun;
typedef struct _funrec * Funrec;
typedef struct _env* Env;
typedef struct _proc* Proc;
typedef struct _procrec* Procrec;

#define ENVSIZE 64
#define MEMSIZE 1024
#define DEFALUT 0xfffffff1

struct {
    int vals[MEMSIZE];
    int ptr;
}memoire;

struct _env{
    char *idents[ENVSIZE];
    Value vals[ENVSIZE];
    int size;
    int cap;
};

struct _value{
    tag_value tag;
    union{
        int val;
        Fun fun;
        Funrec funrec;
        int addr;
        Proc proc;
        Procrec procrec;
    }content;
};

struct  _fun{
    char **args;
    int argc;
    Expr body;
    Env env;
};

struct _funrec{
    char *id;
    Fun fun;
};

struct _proc{
    char **args;
    int argc;
    Cmds block;
    Env env;
};

struct _procrec{
    char *id;
    Proc proc;
};

void free_value(Value v);
// int
Value new_num(int val);
int get_num(Value v);
int nb_args(Args args);
// fun
Value new_fun(int argc, char**args, Expr body);
Value new_funrec(char *id, Fun fun);
Fun get_fun(Value v);
Funrec get_funrec(Value v);
// proc
Value new_proc(int argc, char**args, Cmds body);
Value new_procrec(char *id, Proc proc);
Proc get_proc(Value v);
Procrec get_procrec(Value v);
// memoire
void init_mem();
Value alloc_mem();
int get_addr(Value vaddr);
void modify_mem(Value vaddr, Value val);
//Env
Env new_env();
Value get_env(Env env, char *id);
void add_env(Env env, char *ident, Value v);
void resize_env(Env env, int size);
//Evaluation
Value evalExpr(Expr es, Env env);
void evalDec(Dec dec, Env env);
void evalCmd(Cmd cmd, Env env);
void evalCmds(Cmds cmds, Env env);
void evalProg(Prog prog);