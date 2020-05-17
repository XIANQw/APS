LEX_C = flex
YACC_C = yacc
GCC = gcc

eval: parser ast.h ast.c eval.c prologTerm.c prologTerm.h
	$(GCC) -c ast.c
	$(GCC) -c eval.c
	$(GCC) -c prologTerm.c
	$(GCC) -o evalProg ast.o prologTerm.o eval.o lex.yy.o y.tab.o -lm -ll

parser: lexer.lex parser.y
	$(YACC_C) -d parser.y
	$(LEX_C) lexer.lex
	$(GCC) -c lex.yy.c
	$(GCC) -c y.tab.c

clean:
	rm lex.yy.*
	rm y.tab.*
	rm *.o
	rm evalProg
