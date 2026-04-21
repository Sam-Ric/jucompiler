#!/bin/sh
rm -f jucompiler lex.yy.c y.tab.c y.tab.h
yacc -d -v -t -g --report=all jucompiler.y
lex jucompiler.l
cc -o jucompiler lex.yy.c y.tab.c ast.c -Wall -Wno-unused-function                                                            