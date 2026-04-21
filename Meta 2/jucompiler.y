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
%token<val> BOOLLIT

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
%type<list> MemberList StatementOrVarDecl StatementList IdentifierList
%type<list> FormalParams ParamList ArgList


// Grammar
%%

Program
  : CLASS IDENTIFIER LBRACE MemberList RBRACE {
      $$ = newnode(Program, NULL, line_count, prev_count);
      addchild($$, newnode(Identifier, $2, line_count, prev_col)); // IDENTIFIER
      addchildren($$, $4); // all members from the list MemberList
      root = $$;
    }
  ;

MemberList
  : /* empty */ { $$ = newlist(); }
  | MemberList MethodDecl {
      $$ = $1;
      append($$, $2);
    }
  | MemberList FieldDecl {
      $$ = $1;
      struct node_list *field_list = $2;
      while ((field_list = field_list->next) != NULL) {
        append($$, field_list->node);
      }
      free($2);
    }
  | MemberList SEMICOLON {
      $$ = $1;  // ignore empty statement
    }
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
      append($$, field);

      // create a list for additional IDENTIFIERs
      struct node_list *identifiers = $5;
      while ((identifiers = identifiers->next) != NULL) {
        struct node *extra_field = newnode(FieldDecl, NULL, line_count, prev_col);
        // new Type node for each field
        enum category type_category = $3->category;
        addchild(extra_field, newnode(type_category, NULL, line_count, prev_col));
        addchild(extra_field, identifiers->node); // IDENTIFIER
        append($$, extra_field);
      }
      free($5);
    }
  | error SEMICOLON { $$ = newlist(); }
  ;

IdentifierList
  : /* empty */ { $$ = newlist(); }
  | IdentifierList COMMA IDENTIFIER {
      $$ = $1;
      append($$, newnode(Identifier, $3, line_count, prev_col));
    }
  ;

Type
  : BOOL    { $$ = newnode(Bool, NULL, line_count, prev_col); }
  | INT     { $$ = newnode(Int, NULL, line_count, prev_col); }
  | DOUBLE  { $$ = newnode(Double, NULL, line_count, prev_col); }
  ;

MethodHeader
  : ReturnType IDENTIFIER LPAR RPAR {
      $$ = newnode(MethodHeader, NULL, line_count, prev_col);
      addchild($$, $1);
      addchild($$, newnode(Identifier, $2, line_count, prev_col));
      addchild($$, newnode(MethodParams, NULL, line_count, prev_col)); // empty params
    }
  | ReturnType IDENTIFIER LPAR FormalParams RPAR {
      $$ = newnode(MethodHeader, NULL, line_count, prev_col);
      addchild($$, $1);
      addchild($$, newnode(Identifier, $2, line_count, prev_col));

      // MethodParams node and its params
      struct node *params = newnode(MethodParams, NULL, line_count, prev_col);
      addchildren(params, $4);
      addchild($$, params);
    }
  ;

ReturnType
  : Type { $$ = $1; }
  | VOID { $$ = newnode(Void, NULL, line_count, prev_col); }
  ;

FormalParams
  : ParamList { $$ = $1; }
  | STRING LSQ RSQ IDENTIFIER {
      $$ = newlist();
      struct node *params = newnode(ParamDecl, NULL, line_count, prev_col);
      addchild(param, newnode(StringArray, NULL, line_count, prev_col));
      addchild(param, newnode(Identifier, $4, line_count, prev_col));
      append($$, param);
    }
  ;

ParamList
  : Type IDENTIFIER {
      $$ = newlist();
      struct node *param = newnode(ParamDecl, NULL, line_count, prev_col);
      addchild(param, $1); // Type
      addchild(param, newnode(Identifier, $2, line_count, prev_col)); // IDENTIFIER
      append($$, param);
    }
  | ParamList COMMA Type IDENTIFIER {
      $$ = $1;
      struct node *param = newnode(ParamDecl, NULL, line_count, prev_col);
      addchild(param, $3); // Type
      addchild(param, newnode(Identifier, $4, line_count, prev_col)); // IDENTIFIER
      append($$, param);
    }
  ;

MethodBody
  : LBRACE StatementOrVarDecl RBRACE {
      $$ = newnode(MethodBody, NULL, line_count, prev_col);
      addchildren($$, $2);
    }
  ;

StatementOrVarDecl
  : /* empty */ { $$ = newlist(); }
  | StatementOrVarDecl Statement {
      $$ = $1;
      // add non-NULL statements
      if ($2 != NULL)
        append($$, $2);
    }
  | StatementOrVarDecl VarDecl {
      $$ = $1;
      struct node_list *var_list = $2;
      while ((var_list = var_list->next) != NULL) {
        append($$, var_list->node);
      }
      free($2);
    }
  ;

VarDecl
  : Type IDENTIFIER IdentifierList SEMICOLON {
      $$ = newlist();

      // first Var
      struct node *var = newnode(VarDecl, NULL, line_count, prev_col);
      addchild(var, $1);
      addchild(var, newnode(Identifier, $2, line_count, prev_col));
      append($$, var);

      // additional Vars
      struct node_list *identifiers = $3;
      while ((identifier = identifier->next) != NULL) {
        struct nde *extra_var = newnode(VarDecl, NULL, line_count, prev_col);
        enum category type_category = $1->category;
        addchild(extra_var, newnode(type_category, NULL, line_count, prev_col));
        addchild(extravar, identifier->node);
      }
      free($3);
    }
  ;

Statement
  : LBRACE StatementList RBRACE {
      // check number of statements
      int count = 0;
      struct node_list *temp = $2;
      while ((temp = temp->next) != NULL) count++;

      if (count == 1) {
        $$ = getchild($2, 0);
        free($2);
      } else {
        $$ = newnode(Block, NULL, line_count, prev_col);
        addchildren($$, $2);
      }
    }
  | IF LPAR Expr RPAR Statement %prec LOWER_THAN_ELSE {
      $$ = newnode(If, NULL, line_count, prev_col);
      addchild($$, $3);
      addchild($$, $5);
      addchild($$, newnode(Block, NULL, line_count, prev_col));
    }
  | IF LPAR Expr RPAR Statement ELSE Statement {
      $$ = newnode(If, NULL, line_count, prev_col);
      addchild($$, $3);
      addchild($$, $5);
      addchild($$, $7);
    }
  | WHILE LPAR Expr RPAR Statement {
      $$ = newnode(While, NULL, line_count, prev_col);
      addchild($$, $3);
      addchild($$, $5);
    }
  | RETURN SEMICOLON {
      $$ = newnode(Return, NULL, line_count, prev_col);
    }
  | RETURN Expr SEMICOLON {
      $$ = newnode(Return, NULL, line_count, prev_col);
      addchild($$, $2);
    }
  | SEMICOLON { $$ = NULL; } // empty statement
  | MethodInvocation SEMICOLON { $$ = $1; }
  | Assignment SEMICOLON { $$ = $1; }
  | ParseArgs SEMICOLON { $$ = $1; }
  | PRINT LPAR Expr RPAR SEMICOLON {
      $$ = newnode(Print, NULL, line_count, prev_col);
      addchild($$, $3);
    }
  | PRINT LPAR STRLIT RPAR SEMICOLON {
      $$ = newnode(Print, NULL, line_count, prev_col);
      addchild($$, newnode(StrLit, $3, line_count, prev_col));
    }
  | error SEMICOLON { $$ = NULL; } // errors are not added to the AST
  ;

StatementList
  : /* empty */ { $$ = newlist(); }
  | StatementList Statement {
      $$ = $1;
      if ($2 != NULL)
        append($$, $2);
    }
  ;

MethodInvocation
  : IDENTIFIER LPAR RPAR {
      $$ = newnode(Call, NULL, line_count, prev_col);
      addchild($$, newnode(Identifier, $1, line_count, prev_col));
    }
  | IDENTIFIER LPAR ArgList RPAR {
      $$ = newnode(Call, NULL, line_count, prev_col);
      addchild($$, newnode(Identifier, $1, line_count, prev_col));
      addchildren($$, $3);
    }
  | IDENTIFIER LPAR error RPAR {
      $$ = newnode(Call, NULL, line_count, prev_col);
      addchild($$, newnode(Identifier, $1, line_count, prev_col));
    }
  ;

ArgList
  : Expr {
      $$ = newlist();
      append($$, $1);
    }
  | ArgList COMMA Expr {
      $$ = $1;
      append($$, $3);
    }
  ;

Assignment
  : IDENTIFIER ASSIGN Expr {
      $$ = newnode(Assign, NULL, line_count, prev_col);
      addchild($$, newnode(Identifier, $1, line_count, prev_col));
      addchild($$, $3);
    }
  ;

ParseArgs
  : PARSEINT LPAR IDENTIFIER LSQ Expr RSQ RPAR {
      $$ = newnode(ParseArgs, NULL, line_count, prev_col);
      addchild($$, newnode(Identifier, $3, line_count, prev_col));
      addchild($$, $5);
    }
  | PARSEINT LPAR error RPAR {
      $$ = newnode(ParseArgs, NULL, line_count, prev_col);
    }
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
  | MINUS Expr            %prec UNARY {
      $$ = newnode(Minus, NULL, line_count, prev_col);
      addchild($$, $2);
    }
  | PLUS  Expr            %prec UNARY {
      $$ = newnode(Plus, NULL, line_count, prev_col);
      addchild($$, $2);
    }
  | NOT   Expr            %prec UNARY {
      $$ = newnode(Not, NULL, line_count, prev_col);
      addchild($$, $2);
    }
  | LPAR Expr RPAR { $$ = $2; }
  | LPAR error RPAR {
      // TODO
    }
  | MethodInvocation { $$ = $1; }
  | Assignment { $$ = $1; }
  | ParseArgs { $$ = $1; }
  | IDENTIFIER {
      $$ = newnode(Identifier, $1, line_count, prev_col);
    }
  | IDENTIFIER DOTLENGTH {
      $$ = newnode(Length, NULL, line_count, prev_col);
      addchild($$, newnode(Identifier, $1, line_count, prev_col));
    }
  | NATURAL {
      $$ = newnode(Natural, $1, line_count, prev_col);
    }
  | DECIMAL {
      $$ = newnode(Decimal, $1, line_count, prev_col);
    }
  | BOOLLIT {
      $$ = newnode(BoolLit, $1, line_count, prev_col);
    }
  ;

%%

void yyerror(char *s) {
    printf("Line %d, col %d: %s: %s\n", line_count, prev_col, s, yytext);
}
