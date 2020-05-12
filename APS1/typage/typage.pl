%===============================================================================
%== UPMC/master/info/4I506 -- Janvier 2016                                    ==
%== SU/FSI/master/info/MU4IN503 -- Janvier 2020                               ==
%== Analyse des programmes et sémantiques                                     ==
%===============================================================================
%== Fichier: typrog.pl                                                        ==
%== Vérification de type                                                      ==
%===============================================================================

%%
% Type ::= int | bool | void | arrow([Type],Type)

%%
% Contextes: [(Var,Type)]

% Un contexte de variables -- modifier selon les tests
varCtx([]).

% Contexte initial
primCtx([(add,arrow([int,int],int)),
	(sub,arrow([int,int],int)),
	(mul,arrow([int,int],int)),
	(div,arrow([int,int],int))
       ]).

% Accès
fetchType([(X,T)|_],X,T) :- !.
fetchType([_|G],X,T) :- fetchType(G,X,T).

%%
% Expr
typeExpr(_,num(_),int).

typeExpr(G,sym(X),T) :- fetchType(G,X,T).
    
typeExpr(G,if(E1,E2,E3),T) :-
    typeExpr(G,E1,bool), typeExpr(G,E2,T), typeExpr(G,E3,T).

typeExpr(G,app(F,ES),T) :- typeExpr(G,F,arrow(TS,T)), typeExprs(G,ES,TS).

%
typeExprs(_,[],[]).
typeExprs(G,[E|ES],[T|TS]) :- typeExpr(G,E,T), typeExprs(G,ES,TS).

%
typeStat(G,echo(E),void) :- typeExpr(G,E,int).    

%
typeProg(G,prog(S)) :- typeStat(G,S,void).

%
typeCheck(P,ok) :- varCtx(GP), primCtx(G0), append(GP,G0,G),
		   typeProg(G,P).
typeCheck(_,ko).    


%
exitCode(ok) :- halt(0).
exitCode(_) :- halt(1).

%
main_stdin :-
    read(user_input,T),
    typeCheck(T,R),
    print(R),
    nl,
    exitCode(R).