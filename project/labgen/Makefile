CC=gcc
CFLAGS=-Wall -Wextra

all: main

y.tab.c: labgen.y
	yacc labgen.y

lex.yy.c: labgen.l
	lex labgen.l

main : src/main.c src/lds.c src/points.c src/util.c src/wr.c src/pdt.c src/vars.c y.tab.c lex.yy.c
	$(CC) src/main.c src/lds.c src/points.c src/util.c src/wr.c src/pdt.c src/vars.c y.tab.c -o main

clean:
	rm -f lex.yy.c y.tab.c labgen.o main labres.l labres.y

run: main
	./main
