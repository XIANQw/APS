%{
    #include <stdio.h>
    #include <stdlib.h>
    #include "prologTerm.h"

    int yylex (void);
    int yyerror (char *);
    Cmds theProg;
%}

%token<num>  NUM
%token<str>  IDENT

// bloc
%token  LPAR RPAR LCO RCO
// symbol
%token  NL COLON PV VRG STAR FLECH
%token CONST FUN REC
%token ECHO
//type
%token INT BOOL
// logic
%token TRUE FALSE NOT AND OR
// op
%token  EQ LT GT PLUS MINUS MULT DIV

%token IF

%union {
    int num;
    char* str;
    Expr expr;
    Exprs exprs;
    Cmds cmds;
    Cmd cmd;
    Type type;
    cbool bool;
    Oprim oprim;
}

%type<cmds> prog
%type<cmd> stat
%type<cmd> dec
%type<cmd> cmd
%type<cmds> cmds
%type<expr> expr
%type<exprs> exprs
%type<type> type
%type<bool> bool
%type<oprim> oprim


%start prog
%%

prog:
LCO cmds RCO {theProg = $2; }
;

cmd:
stat {$$ = $1;}
| dec {$$ = $1; }
;

cmds:
cmd {$$ = appendCmds($1, NULL); }
| cmd PV cmds {$$ = appendCmds($1, $3); } 
;

stat:
ECHO expr { $$ = newASTStat($2);}
;

dec:
CONST IDENT type expr {$$ = newASTDec($2, $3, $4); }
;

type:
INT {$$ = T_INT; }
| BOOL {$$ = T_BOOL; }
;

bool:
TRUE {$$ = c_true; }
| FALSE {$$ = c_false; }
;

oprim:
PLUS {$$ = Add; }
| MINUS {$$ = Sub; }
| MULT {$$ = Mul; }
| DIV {$$ = Div; }
| OR {$$ = Or; }
| AND {$$ = And; }
| EQ {$$ = Eq; }
| LT {$$ = Lt; }
| GT {$$ = Gt; }
;

expr:
NUM                       { $$ = newASTNum($1); }
| bool                    { $$ = newASTBool($1); }
| IDENT                     { $$ = newASTId($1); }
| LPAR oprim exprs RPAR   { $$ = newASTPrim($2,$3); }
;

exprs:
expr { $$ = appendExprs($1,NULL); }
| expr exprs { $$ = appendExprs($1,$2); }
;


%%

int yyerror(char *s) {
    printf("error: %s\n",s);
    return 1;
}

int main(int argc, char **argv) {
    yyparse();
    printCmds(theProg);
    printf("\n");
    return 0;
}
