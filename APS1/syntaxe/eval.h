#include "ast.h"

typedef _env* Env;

typedef struct{
    char *idents[1024];
    int values[1024];
    size_t size;
    size_t capacity;
}_env;

Env new_env();
void add_env(Env env, char *ident, int value);
int get_env(Env env, char *ident);

int evalExpr(Expr es, Env env);
void evalCmd(Cmd cmd, Env env);
void evalProg(Prog prog);