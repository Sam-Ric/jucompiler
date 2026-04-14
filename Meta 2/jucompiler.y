/*
  João Tomás Correia Ferreira (2023217920)
  Samuel Marques Riça (2023206471)
*/
%{
#include <stdio.h>
extern int yylex(void);;
void yyerror(char *);
extern char *yytext;  
%}

// Tokens
%token LPAR
%token RPAR
%token LBRACE
%token RBRACE
%token LSQ
%token RSQ
%token EQ
%token ASSIGN
%token GE
%token GT
%token LE
%token LT
%token NE
%token NOT
%token COMMA
%token SEMICOLON

%token STAR
%token DIV
%token PLUS
%token MINUS
%token MOD

%token AND
%token OR
%token ARROW
%token LSHIFT
%token RSHIFT
%token XOR

%token INT
%token DOUBLE
%token BOOL
%token STRING

%token IF
%token ELSE

%token BOOLIT

%token CLASS
%token PUBLIC
%token RETURN
%token STATIC
%token VOID
%token WHILE

%token RESERVED
%token DOTLENGTH
%token PRINT
%token PARSEINT

%token NATURAL
%token DECIMAL
%token IDENTIFIER

// Grammar
%%

jucompiler: Program                 { printf("%d\n", )}

%%

void yyerror(char *error) {
    printf("%s '%s'\n", error, yytext);
}