%{
    #include <stdio.h>
    #include <stdlib.h>
    #include "prologTerm.h"

    int yylex (void);
    int yyerror (char *);
    Prog theProg;
%}

%token<num>  NUM
%token<str>  IDENT

// bloc
%token  LPAR RPAR LCO RCO
// symbol
%token  NL COLON PV VRG STAR FLECH
// dec
%token CONST FUN REC PROC VAR
// stat
%token ECHO
// type
%token INT BOOL VOID
// logic
%token TRUE FALSE NOT AND OR
// op
%token  EQ LT GT PLUS MINUS MULT DIV

%token IF SET WHILE CALL IF_PROC

%union {
    int num;
    char* str;
    Expr expr;
    Exprs exprs;
    Dec dec;
    Cmds cmds;
    Cmd cmd;
    Stat stat;
    Tprim tprim;
    Type type;
    Types types;
    cbool bool;
    Oprim oprim;
    Arg arg;
    Args args;
    Prog prog;
}

%type<prog> prog
%type<stat> stat
%type<dec> dec
%type<cmd> cmd
%type<cmds> cmds
%type<expr> expr
%type<exprs> exprs
%type<bool> bool
%type<tprim> tprim
%type<type> type;
%type<types> types; 
%type<oprim> oprim;
%type<arg> arg;
%type<args> args;


%start prog
%%

prog:
LCO cmds RCO {theProg = newASTProg($2); }
;

cmd:
stat {$$ = newASTCmdStat($1);}
| dec {$$ = newASTCmdDec($1); }
;

cmds:
cmd {$$ = appendCmds($1, NULL); }
| cmd PV cmds {$$ = appendCmds($1, $3); } 
;

stat:
ECHO expr { $$ = newASTEcho($2);}
|SET IDENT expr {$$ = newASTSet($2, $3);}
;

dec:
CONST IDENT type expr 
{$$ = newASTDec($2, $3, NULL, $4, DEC_CONS); }
|FUN IDENT type LCO args RCO expr 
{$$ = newASTDec($2, $3, $5, $7, DEC_FUN);}
|FUN REC IDENT type LCO args RCO expr 
{$$ = newASTDec($3, $4, $6, $8, DEC_FUNREC);}
|VAR IDENT type 
{$$ = newASTDec($2, $3, NULL, NULL, DEC_VAR);}
|PROC IDENT LCO args RCO prog 
{$$ = newASTDecProc($2, $4, $6, DEC_PROC);}
|PROC REC IDENT LCO args RCO prog
{$$ = newASTDecProc($3, $5, $7, DEC_PROCREC);}
;

bool:
TRUE {$$ = c_true; }
| FALSE {$$ = c_false; }
;

tprim:
INT {$$ = T_INT; }
| BOOL {$$ = T_BOOL; }
| VOID {$$ = T_VOID; }
;

type:
tprim {$$ = newASTType1($1);}
| LPAR types FLECH type RPAR {$$ = newASTType2($2, $4);}
;

types:
type {$$ = appendTypes($1, NULL);}
| type STAR types {$$ = appendTypes($1, $3);}
;

arg:
IDENT COLON type {$$ = newASTArg($1, $3);}
;

args:
arg {$$ = appendArgs($1, NULL);}
|arg VRG args {$$ = appendArgs($1,$3);}
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
| LPAR IF expr expr expr RPAR {$$ = newASTIf($3, $4, $5);}
| LCO args RCO expr {$$ = newASTLambda($2, $4);}
| LPAR exprs RPAR {$$ = newASTBloc($2); }
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
    printProg(theProg);
    printf("\n");
    return 0;
}
