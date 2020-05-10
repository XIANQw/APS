LEX_C = flex
YACC_C = yacc
GCC = gcc

prologTerm: parser ast.h ast.c prologTerm.c
	$(GCC) -c ast.c
	$(GCC) -c prologTerm.c
	$(GCC) -o prologTermProg ast.o prologTerm.o lex.yy.o y.tab.o -lm -ll

parser: lexer.lex parser.y
	$(YACC_C) -d parser.y
	$(LEX_C) lexer.lex
	$(GCC) -c lex.yy.c
	$(GCC) -c y.tab.c

clean:
	rm lex.yy.*
	rm y.tab.*
	rm *.o
	rm prologTermProg
