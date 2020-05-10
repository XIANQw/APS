#include "ast.h"

typedef struct{
    char *idents[64];
    int values[64];
    char *funident[64];
    Dec func[64];
    size_t size;
    size_t funsize;
    size_t cap;
}_env;
typedef _env* Env;

Env new_env();
void add_env(Env env, char *ident, int value);
int get_env(Env env, char *ident);

int evalExpr(Expr es, Env env);
void evalCmd(Cmd cmd, Env env);
void evalProg(Prog prog);