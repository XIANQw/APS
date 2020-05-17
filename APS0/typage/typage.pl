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
expr(G,gt(X,Y),bool):-expr(G,X,int),expr(G,Y,int).
expr(G,lt(X,Y),bool):-expr(G,X,int),expr(G,Y,int).
expr(G,add(X,Y),int):-expr(G,X,int),expr(G,Y,int).
expr(G,sub(X,Y),int):-expr(G,X,int),expr(G,Y,int).
expr(G,mul(X,Y),int):-expr(G,X,int),expr(G,Y,int).
expr(G,div(X,Y),int):-expr(G,X,int),expr(G,Y,int).

% if
expr(G,if(E1,E2,E3),Type):-expr(G,E1,bool),expr(G,E2,Type),expr(G,E3,Type).
expr(G,X,Type):-member((X,Type),G).


% Application
expr(G,appFunc(F,ExprList),Type):-expr(G,F,func(TypeList,Type)),exprs(G,ExprList,TypeList).

% ABS
expr(G,lambda(ARGS,E),func(TS,Type)):-getTypes(ARGS,TS),append(G,ARGS,GG),expr(GG,E,Type).

% echo
stat(G,echo(Expr),void):-expr(G,Expr,int).

% Déclaration
% const
dec(G,const(Id,Type,Expr),[(Id,Type)|G]):-expr(G,Expr,Type).

% fonction
dec(G,fun(Id,Type,ARGS,Expr),[(Id,func(TS,Type))|G]):-append(G,ARGS,GG),expr(GG,Expr,Type),getTypes(ARGS,TS).

% fonction récursive
dec(G,funRec(Id,Type,ARGS,Expr),[(Id,func(TS,Type))|G]):-append(G,ARGS,G2),append(G2,[(Id,func(TS,Type))],GG),expr(GG,Expr,Type),getTypes(ARGS,TS).


% Suites de commandes
% commence par une déclaration
cmds(G,[DEC|CMDS],void):-dec(G,DEC,G2),cmds(G2,CMDS,void).
% commence par un statement
cmds(G,[STAT|CMDS],void):-stat(G,STAT,void),cmds(G,CMDS,void).
% vide
cmds(_,[epsilon],void).

% Programmes
program(program(CMDS),void):-append(CMDS,[epsilon],L),cmds([],L,void).

prog(P,true):-program(P,void).

main_stdin :-
    read(user_input,Type),
    prog(Type,R),
    print(R).
