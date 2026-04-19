/*
  João Tomás Correia Ferreira (2023217920)
  Samuel Marques Riça (2023206471)
*/
%{
#include <stdio.h>
extern int yylex(void);;
void yyerror(char *);
extern char *yytext;
extern int line_count, col_count;
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
%token STRING
%token BOOL

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

// Precedências

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%left OR 
%left XOR 
%left AND 
%left EQ NE 
%left LT LE GT GE 
%left LSHIFT RSHIFT
%left PLUS MINUS 
%left STAR DIV MOD 

%nonassoc UNARY

// Grammar
%%

Program
  : CLASS IDENTIFIER LBRACE MemberList RBRACE
  ;

MemberList
  : 
  | MemberList MethodDecl
  | MemberList FieldDecl
  | MemberList SEMICOLON
  ;

MethodDecl
  : PUBLIC STATIC MethodHeader MethodBody
  ;

FieldDecl
  : PUBLIC STATIC Type IDENTIFIER IdentifierList SEMICOLON
  | error SEMICOLON
  ;

IdentifierList
  :
  | IdentifierList COMMA IDENTIFIER
  ;

Type
  : BOOL
  | INT
  | DOUBLE
  ;

MethodHeader
  : ReturnType IDENTIFIER LPAR RPAR 
  | ReturnType IDENTIFIER LPAR FormalParams RPAR
  ;

ReturnType
  : Type 
  | VOID 
  ;

FormalParams
  : ParamList 
  | STRING LSQ RSQ IDENTIFIER
  ;

ParamList
  : Type IDENTIFIER
  | ParamList COMMA Type IDENTIFIER
  ;

MethodBody
  : LBRACE StatementOrVarDecl RBRACE 
  ;

StatementOrVarDecl
  :
  | StatementOrVarDecl Statement 
  | StatementOrVarDecl VarDecl 
  ;

VarDecl
  : Type IDENTIFIER IdentifierList SEMICOLON
  ;

Statement
  : LBRACE StatementList RBRACE
  | IF LPAR Expr RPAR Statement                   %prec LOWER_THAN_ELSE
  | IF LPAR Expr RPAR Statement ELSE Statement
  | WHILE LPAR Expr RPAR Statement
  | RETURN SEMICOLON
  | RETURN Expr SEMICOLON
  | SEMICOLON
  | MethodInvocation SEMICOLON
  | Assignment SEMICOLON
  | ParseArgs SEMICOLON
  | PRINT LPAR Expr RPAR SEMICOLON
  | PRINT LPAR STRING RPAR SEMICOLON
  | error SEMICOLON
  ;

StatementList
  : 
  | StatementList Statement
  ;

MethodInvocation
  : IDENTIFIER LPAR RPAR
  | IDENTIFIER LPAR ArgList RPAR
  | IDENTIFIER LPAR error RPAR
  ;

ArgList
  : Expr
  | ArgList COMMA Expr
  ;

Assignment
  : IDENTIFIER ASSIGN Expr
  ;

ParseArgs
  : PARSEINT LPAR IDENTIFIER LSQ Expr RSQ RPAR
  | PARSEINT LPAR error RPAR
  ;

Expr
  : Expr PLUS   Expr
  | Expr MINUS  Expr
  | Expr STAR   Expr
  | Expr DIV    Expr
  | Expr MOD    Expr
  | Expr AND    Expr
  | Expr OR     Expr
  | Expr XOR    Expr
  | Expr LSHIFT Expr
  | Expr RSHIFT Expr
  | Expr EQ     Expr
  | Expr GE     Expr
  | Expr GT     Expr
  | Expr LE     Expr
  | Expr LT     Expr
  | Expr NE     Expr
  | MINUS Expr            %prec UNARY
  | PLUS  Expr            %prec UNARY
  | NOT   Expr            %prec UNARY
  | LPAR Expr RPAR
  | LPAR error RPAR
  | MethodInvocation
  | Assignment
  | ParseArgs
  | IDENTIFIER
  | IDENTIFIER DOTLENGTH
  | NATURAL
  | DECIMAL
  | BOOLIT
  ;

%%

void yyerror(char *s) {
    printf("Line %d, col %d: %s: %s\n", line_count, col_count, s, yytext);
}