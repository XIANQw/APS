#include "ast.h"

typedef enum {
    V_INT, V_FUN, V_FUNREC
}tag_value;

typedef struct _value * Value;
typedef struct _fun * Fun;
typedef struct _funrec * Funrec;

struct _value{
    tag_value tag;
    union{
        int val;
        Fun fun;
        Funrec funrec;
    }content;
};

struct  _fun{
    char **args;
    int argc;
    Expr body;
};

struct _funrec{
    char *id;
    Fun fun;
};

void free_value(Value v);
Value new_num(int val);
int get_num(Value v);
int nb_args(Args args);
Value new_fun(int argc, char**args, Expr body);
Value new_funrec(char *id, Fun fun);
Fun get_fun(Value v);
Funrec get_funrec(Value v);

#define ENVSIZE 64

typedef struct _env* Env;

struct _env{
    char *idents[ENVSIZE];
    Value vals[ENVSIZE];
    int size;
    int cap;
};

Env new_env();
Value get_env(Env env, char *id);
void add_env(Env env, char *ident, Value v);
void resize_env(Env env, int size);
Value evalExpr(Expr es, Env env);
void evalCmd(Cmd cmd, Env env);
void evalProg(Prog prog);