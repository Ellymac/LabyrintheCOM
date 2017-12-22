#!/bin/sh

# Compile
yacc labres1.y
lex labres1.l
gcc y.tab.c

# Execute with .dat files
for f in datfiles/*.dat 
    do
        echo "$f : "
        ./a.out < "$f"
    done

# Clean
rm y.tab.c lex.yy.c a.out 
