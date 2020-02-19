.PHONY = clean run

LEX_C = lex
YACC_C = yacc
CC = gcc
SRC = src
EXC = exc
BIN = bin
CPPFLAGS= -I $(SRC)/include
PARSER= $(SRC)/lexer/lex.yy.c $(SRC)/parser/y.tab.c

vpath %.o $(BIN)
vpath %.c $(SRC)/ast
vpath %.h $(SRC)/include
vpath %.c $(SRC)/prog
vpath %.y $(SRC)/parser
vpath %.lex $(SRC)/lexer


All: $(PARSER) $(EXC)/prologTerm

obj := $(patsubst %.c,%.o,$(wildcard $(SRC)/ast/*.c))
obj += $(patsubst %.c,%.o,$(wildcard $(SRC)/prog/*.c))


$(EXC)/prologTerm: $(obj) $(SRC)/parser/y.tab.o $(SRC)/lexer/lex.yy.o
	$(CC) -o $@ $^ -lm -ll


$(SRC)/parser/y.tab.c: $(SRC)/parser/parser.y
	$(YACC_C) -d $<
	mv y.tab.h $(SRC)/include/
	mv y.tab.c $(SRC)/parser/
$(SRC)/lexer/lex.yy.c: $(SRC)/lexer/lexer.lex
	$(LEX_C) -o $@ $<

%.o: %.c
	$(CC) $(CPPFLAGS) -c $< -o $@

run: $(EXC)/prologTerm
	./$(EXC)/prologTerm < in

clean: 
	rm $(SRC)/lexer/lex.yy.*;
	rm $(SRC)/parser/y.tab.*;
	rm $(SRC)/include/y.tab.*;
	rm $(obj);
	rm $(EXC)/prologTerm;