%{
    #include <stdlib.h>
    #include "ast.h"
    #include "y.tab.h"
%}


nls "\n"|"\r"|"\r\n"
nums "-"?[0-9]+
idents [a-zA-Z][a-zA-Z0-9]*

%%
[ \t]  { /* On ignore */ }
{nls}  {  }

"(" return(LPAR);
")" return(RPAR);
"[" return(LCO);
"]" return(RCO);
":" return(COLON);
";" return(PV);
"," return(VRG);
"*" return(STAR);
"->" return(FLECH);

"CONST" return(CONST);
"FUN" return(FUN);
"REC" return(REC);

"echo" return(ECHO);
"ECHO" return(ECHO);

"bool" return(BOOL);
"int" return(INT);

"true" return(TRUE);
"false" return(FALSE);
"not" return(NOT);
"and" return(AND);
"or" return(OR);

"eq" return(EQ);
"lt" return(LT);
"gt" return(GT);
"add" return(PLUS);
"sub" return(MINUS);
"mul" return(MULT);
"div" return(DIV);

"if" return(IF);




{nums}    {
    yylval.num=atoi(yytext);
    return(NUM);
}

{idents}    {
    yylval.str=strdup(yytext);
    return(IDENT);
}

