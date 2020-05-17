member(X,[X|_]).
member(X,[_|Z]):-member(X,Z).
assoc(K,[(K,V)|_],V).
assoc(K,[_|L],V):-assoc(K,L,V).
append([],L,L).
append([X|Y],L,[X|L2]):-append(Y,L,L2).

exprs(_,[],[]).
exprs(G,[Expr|ExprList],[Type|TypeList]):-expr(G,Expr,Type),exprs(G,ExprList,TypeList).

getTypes([(_,Type)],[Type]).
getTypes([(_,Type)|ARGS],[Type|TypeList]):-getTypes(ARGS,TypeList).


/*********************APS0********************/

/*Expression*/
% bool
expr(_,true,bool).
expr(_,false,bool).
% num
expr(_,X,int):-integer(X).
% primitives
expr(G,not(X),bool):-expr(G,X,bool).
expr(G,and(X,Y),bool):-expr(G,X,bool),expr(G,Y,bool).
expr(G,or(X,Y),bool):-expr(G,X,bool),expr(G,Y,bool).
expr(G,eq(X,Y),bool):-expr(G,X,int),expr(G,Y,int).
expr(G,lt(X,Y),bool):-expr(G,X,int),expr(G,Y,int).
expr(G,add(X,Y),int):-expr(G,X,int),expr(G,Y,int).
expr(G,sub(X,Y),int):-expr(G,X,int),expr(G,Y,int).
expr(G,mul(X,Y),int):-expr(G,X,int),expr(G,Y,int).
expr(G,div(X,Y),int):-expr(G,X,int),expr(G,Y,int).
% if
expr(G,if(E1,E2,E3),T):-expr(G,E1,bool),expr(G,E2,T),expr(G,E3,T).
expr(G,X,T):-member((X,T),G).

% Application
expr(G,appFunc(F,ExprList),Type):-expr(G,F,func(TypeList,Type)),exprs(G,ExprList,TypeList).

% ABS
expr(G,lambda(ARGS,E),func(TS,Type)):-getTypes(ARGS,TS),append(G,ARGS,GG),expr(GG,E,Type).


% Suites de commandes
% commence par une déclaration
cmds(G,[DEC|CMDS],void):-dec(G,DEC,G2),cmds(G2,CMDS,void).
% commence par un statement
cmds(G,[STAT|CMDS],void):-stat(G,STAT,void),cmds(G,CMDS,void).
% vide
cmds(_,[epsilon],void).

% Programmes
program(program(CMDS),void):-append(CMDS,[epsilon],L),cmds([],L,void).


/*********************APS1********************/

% block
block(G,CMDS,void):-append(CMDS,[epsilon],L),cmds(G,L,void).

% Déclaration
% const
dec(G,const(Id,Type,Expr),[(Id,Type)|G]):-expr(G,Expr,Type).

% fonction
dec(G,fun(Id,Type,ARGS,Expr),[(Id,func(TS,Type))|G]):-append(G,ARGS,GG),expr(GG,Expr,Type),getTypes(ARGS,TS).

% fonction récursive
dec(G,funRec(Id,Type,ARGS,Expr),[(Id,func(TS,Type))|G]):-append(G,ARGS,G2),append(G2,[(Id,func(TS,Type))],GG),expr(GG,Expr,Type),getTypes(ARGS,TS).

% variable
dec(G,var(Id,Type),[(Id,Type)|G]).

% procédures
dec(G,proc(Id,ARGS,BLOCK),[(Id,func(LT,void))|G]):-append(G,ARGS,GG),block(GG,BLOCK,void),getTypes(ARGS,LT).

% procédures récursive
dec(G,procRec(Id,ARGS,BLOCK),[(Id,func(LT,void))|G]):- append(G,ARGS,G2),append(G2,[(Id,func(LT,void))],GG),block(GG,BLOCK,void),getTypes(ARGS,LT).

% statement
% echo
stat(G,echo(Expr),void):-expr(G,Expr,int).

% set
stat(G,set(Id,Expr),void):-expr(G,Id,Type),expr(G,Expr,Type).

% ifstat
stat(G,ifstat(COND,BLOCK1,BLOCK2),void):-expr(G,COND,bool),block(G,BLOCK1,void),block(G,BLOCK2,void).

% while
stat(G,while(COND,BLOCK),void):-expr(G,COND,bool),block(G,BLOCK,void).

% call
stat(G,call(Fun,Exprs),void):-exprs(G,Exprs,Types),expr(G,Fun,func(Types,void)).



prog(P,true):-program(P,void).

main_stdin :-
read(user_input,T),
prog(T,R),
print(R).



