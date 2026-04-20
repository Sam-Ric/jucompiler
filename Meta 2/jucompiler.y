/*
  João Tomás Correia Ferreira (2023217920)
  Samuel Marques Riça (2023206471)
*/
%{
  #include <stdio.h>
  #include "ast.h"

  extern int yylex(void);;
  void yyerror(char *);
  extern char *yytext;
  extern int line_count, prev_col;

  struct node *root = NULL; // AST root
  int syntax_errors = 0;    // error counter
%}

%union {
  char *val;              // tokens with string values
  struct node *node;      // AST nodes
  struct node_list *list; // list of nodes
}

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

%token<val> NATURAL
%token<val> DECIMAL
%token<val> IDENTIFIER
%token<val> STRLIT
%token<val> BOOLIT

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

// Non-terminal symbols
%type<node> Program MethodDecl FieldDecl MethodHeader MethodBody
%type<node> Type ReturnType VarDecl Statement MethodInvocation
%type<node> Assignment ParseArgs Expr
%type<node> MemberList StatementOrVarDecl StatementList IdentifierList
%type<node> FormalParams ParamList ArgList


// Grammar
%%

Program
  : CLASS IDENTIFIER LBRACE MemberList RBRACE {
      $$ = newnode(Program, NULL, line_count, prev_count);
      addchild($$, newnode(Identifier, $2, line_count, prev_col)); // IDENTIFIER
      addchild($$, $4); // MemberList
      root = $$;
    }
  ;

MemberList
  : 
  | MemberList MethodDecl
  | MemberList FieldDecl
  | MemberList SEMICOLON
  ;

MethodDecl
  : PUBLIC STATIC MethodHeader MethodBody {
      $$ = newnode(MethodDecl, NULL, line_count, prev_col);
      addchild($$, $3); // MethodHeader
      addchild($$, $4); // MethodBody
    }
  ;

FieldDecl
  : PUBLIC STATIC Type IDENTIFIER IdentifierList SEMICOLON {
      // first IDENTIFIER
      struct node *field = newnode(FieldDecl, NULL, line_count, prev_col);
      addchild(field, $3);  // Type
      addchild(field, newnode(Identifier, $4, line_count, prev_col));  // IDENTIFIER

      // create a list for additional IDENTIFIERs
      struct node_list *identifiers = $5;
      while ((identifiers = identifiers->next) != NULL) {
        struct node *extra_field = newnode(FieldDecl, NULL, line_count, prev_col);
        addchild(extra_field, $3); // same Type
        addchild(extra_field, identifiers->node); // IDENTIFIER
        addchildren($$, extra_field);
      }
    }
  | error SEMICOLON { $$ = newlist(); }
  ;

IdentifierList
  : /* empty */ { $$ = newlist(); }
  | IdentifierList COMMA IDENTIFIER {
      $$ = $1;
      addchildren($$, newnode(Identifier, $3, line_count, prev_col));
    }
  ;

Type
  : BOOL    { $$ = newnode(Bool, NULL, line_count, prev_col); }
  | INT     { $$ = newnode(Int, NULL, line_count, prev_col); }
  | DOUBLE  { $$ = newnode(Double, NULL, line_count, prev_col); }
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
  : Expr PLUS   Expr {
      $$ = newnode(Add, NULL, line_count, prev_col);
      addchild($$, $1); // Expr1
      addchild($$, $3); // Expr2
    }
  | Expr MINUS  Expr {
      $$ = newnode(Sub, NULL, line_count, prev_col);
      addchild($$, $1); // Expr1
      addchild($$, $3); // Expr2
    }
  | Expr STAR   Expr {
      $$ = newnode(Mul, NULL, line_count, prev_col);
      addchild($$, $1); // Expr1
      addchild($$, $3); // Expr2
    }
  | Expr DIV    Expr {
      $$ = newnode(Div, NULL, line_count, prev_col);
      addchild($$, $1); // Expr1
      addchild($$, $3); // Expr2
    }
  | Expr MOD    Expr {
      $$ = newnode(Mod, NULL, line_count, prev_col);
      addchild($$, $1); // Expr1
      addchild($$, $3); // Expr2
    }
  | Expr AND    Expr {
      $$ = newnode(And, NULL, line_count, prev_col);
      addchild($$, $1); // Expr1
      addchild($$, $3); // Expr2
    }
  | Expr OR     Expr {
      $$ = newnode(Or, NULL, line_count, prev_col);
      addchild($$, $1); // Expr1
      addchild($$, $3); // Expr2
    }
  | Expr XOR    Expr {
      $$ = newnode(Xor, NULL, line_count, prev_col);
      addchild($$, $1); // Expr1
      addchild($$, $3); // Expr2
    }
  | Expr LSHIFT Expr {
      $$ = newnode(Lshift, NULL, line_count, prev_col);
      addchild($$, $1); // Expr1
      addchild($$, $3); // Expr2
    }
  | Expr RSHIFT Expr {
      $$ = newnode(Rshift, NULL, line_count, prev_col);
      addchild($$, $1); // Expr1
      addchild($$, $3); // Expr2
    }
  | Expr EQ     Expr {
      $$ = newnode(Eq, NULL, line_count, prev_col);
      addchild($$, $1); // Expr1
      addchild($$, $3); // Expr2
    }
  | Expr GE     Expr {
      $$ = newnode(Ge, NULL, line_count, prev_col);
      addchild($$, $1); // Expr1
      addchild($$, $3); // Expr2
    }
  | Expr GT     Expr {
      $$ = newnode(Gt, NULL, line_count, prev_col);
      addchild($$, $1); // Expr1
      addchild($$, $3); // Expr2
    }
  | Expr LE     Expr {
      $$ = newnode(Le, NULL, line_count, prev_col);
      addchild($$, $1); // Expr1
      addchild($$, $3); // Expr2
    }
  | Expr LT     Expr {
      $$ = newnode(Lt, NULL, line_count, prev_col);
      addchild($$, $1); // Expr1
      addchild($$, $3); // Expr2
    }
  | Expr NE     Expr {
      $$ = newnode(Ne, NULL, line_count, prev_col);
      addchild($$, $1); // Expr1
      addchild($$, $3); // Expr2
    }
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
    printf("Line %d, col %d: %s: %s\n", line_count, prev_col, s, yytext);
}
