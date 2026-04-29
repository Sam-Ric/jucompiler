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
  extern char last_token_text[];
  extern int line_count, col_count, string_col;

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

%locations

// Precedências

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%right ASSIGN

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
%type<node> Program MethodDecl MethodHeader MethodBody
%type<node> Type Statement MethodInvocation
%type<node> Assignment ParseArgs Expr
%type<list> MemberList StatementOrVarDecl StatementList IdentifierList
%type<list> FormalParams ParamList ArgList FieldDecl VarDecl


// Grammar
%%

Program
  : CLASS IDENTIFIER LBRACE MemberList RBRACE {
      $$ = newnode(Program, NULL, @1.first_line, @1.first_column);
      addchild($$, newnode(Identifier, $2, @2.first_line, @2.first_column)); // IDENTIFIER
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
      $$ = newnode(MethodDecl, NULL, @1.first_line, @1.first_column);
      addchild($$, $3); // MethodHeader
      addchild($$, $4); // MethodBody
    }
  ;

FieldDecl
  : PUBLIC STATIC Type IDENTIFIER IdentifierList SEMICOLON {
      // first IDENTIFIER
      $$ = newlist();
      struct node *field = newnode(FieldDecl, NULL,  @1.first_line, @1.first_column);
      addchild(field, $3);  // Type
      addchild(field, newnode(Identifier, $4, @4.first_line, @4.first_column));  // IDENTIFIER
      append($$, field);

      // create a list for additional IDENTIFIERs
      struct node_list *identifiers = $5;
      while ((identifiers = identifiers->next) != NULL) {
        struct node *extra_field = newnode(FieldDecl, NULL, @1.first_line, @1.first_column);
        // new Type node for each field
        enum category type_category = $3->category;
        addchild(extra_field, newnode(type_category, NULL, @1.first_line, @1.first_column));
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
      append($$, newnode(Identifier, $3, @3.first_line, @3.first_column));
    }
  ;

Type
  : BOOL    { $$ = newnode(Bool, NULL, @1.first_line, @1.first_column); }
  | INT     { $$ = newnode(Int, NULL, @1.first_line, @1.first_column); }
  | DOUBLE  { $$ = newnode(Double, NULL, @1.first_line, @1.first_column); }
  ;

MethodHeader
  : Type IDENTIFIER LPAR RPAR {
      $$ = newnode(MethodHeader, NULL, @1.first_line, @1.first_column);
      addchild($$, $1);
      addchild($$, newnode(Identifier, $2, @2.first_line, @2.first_column));
      addchild($$, newnode(MethodParams, NULL, @3.first_line, @3.first_column));
    }
  | Type IDENTIFIER LPAR FormalParams RPAR {
      $$ = newnode(MethodHeader, NULL, @1.first_line, @1.first_column);
      addchild($$, $1);
      addchild($$, newnode(Identifier, $2, @2.first_line, @2.first_column));
      struct node *params = newnode(MethodParams, NULL, @3.first_line, @3.first_column);
      addchildren(params, $4);
      addchild($$, params);
    }
  | VOID IDENTIFIER LPAR RPAR {
      $$ = newnode(MethodHeader, NULL, @1.first_line, @1.first_column);
      addchild($$, newnode(Void, NULL, @1.first_line, @1.first_column));
      addchild($$, newnode(Identifier, $2, @2.first_line, @2.first_column));
      addchild($$, newnode(MethodParams, NULL, @3.first_line, @3.first_column));
    }
  | VOID IDENTIFIER LPAR FormalParams RPAR {
      $$ = newnode(MethodHeader, NULL, @1.first_line, @1.first_column);
      addchild($$, newnode(Void, NULL, @1.first_line, @1.first_column));
      addchild($$, newnode(Identifier, $2, @2.first_line, @2.first_column));
      struct node *params = newnode(MethodParams, NULL, @3.first_line, @3.first_column);
      addchildren(params, $4);
      addchild($$, params);
    }
  ;

FormalParams
  : ParamList { $$ = $1; }
  | STRING LSQ RSQ IDENTIFIER {
      $$ = newlist();
      struct node *param = newnode(ParamDecl, NULL, @1.first_line, @1.first_column);
      addchild(param, newnode(StringArray, NULL, @1.first_line, @1.first_column));
      addchild(param, newnode(Identifier, $4, @4.first_line, @4.first_column));
      append($$, param);
    }
  ;

ParamList
  : Type IDENTIFIER {
      $$ = newlist();
      struct node *param = newnode(ParamDecl, NULL, @1.first_line, @1.first_column);
      addchild(param, $1); // Type
      addchild(param, newnode(Identifier, $2, @2.first_line, @2.first_column)); // IDENTIFIER
      append($$, param);
    }
  | ParamList COMMA Type IDENTIFIER {
      $$ = $1;
      struct node *param = newnode(ParamDecl, NULL, @3.first_line, @3.first_column);
      addchild(param, $3); // Type
      addchild(param, newnode(Identifier, $4, @4.first_line, @4.first_column)); // IDENTIFIER
      append($$, param);
    }
  ;

MethodBody
  : LBRACE StatementOrVarDecl RBRACE {
      $$ = newnode(MethodBody, NULL, @1.first_line, @1.first_column);
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
      struct node *var = newnode(VarDecl, NULL, @1.first_line, @1.first_column);
      addchild(var, $1);
      addchild(var, newnode(Identifier, $2, @2.first_line, @2.first_column));
      append($$, var);

      // additional Vars
      struct node_list *identifiers = $3;
      while ((identifiers = identifiers->next) != NULL) {
        struct node *extra_var = newnode(VarDecl, NULL, @1.first_line, @1.first_column);
        enum category type_category = $1->category;
        addchild(extra_var, newnode(type_category, NULL, @1.first_line, @1.first_column));
        addchild(extra_var, identifiers->node);
        append($$, extra_var);
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

      if (count == 0) {
        $$ = NULL;
      } else if (count == 1) {
        $$ = $2->next->node;
        free($2);
      } else {
        $$ = newnode(Block, NULL, @1.first_line, @1.first_column);
        addchildren($$, $2);
      }
    }
  | IF LPAR Expr RPAR Statement %prec LOWER_THAN_ELSE {
      $$ = newnode(If, NULL, @1.first_line, @1.first_column);
      addchild($$, $3);
      if ($5 != NULL)
        addchild($$, $5);
      else
        addchild($$, newnode(Block, NULL, @1.first_line, @1.first_column));
      addchild($$, newnode(Block, NULL, @1.first_line, @1.first_column));
    }
  | IF LPAR Expr RPAR Statement ELSE Statement {
      $$ = newnode(If, NULL, @1.first_line, @1.first_column);
      addchild($$, $3);
      if ($5 != NULL)
        addchild($$, $5);
      else
        addchild($$, newnode(Block, NULL, @1.first_line, @1.first_column));
      if ($7 != NULL)
        addchild($$, $7);
      else
        addchild($$, newnode(Block, NULL, @1.first_line, @1.first_column));
    }
  | WHILE LPAR Expr RPAR Statement {
      $$ = newnode(While, NULL, @1.first_line, @1.first_column);
      addchild($$, $3);
      if ($5 != NULL)
        addchild($$, $5);
      else
        addchild($$, newnode(Block, NULL, @1.first_line, @1.first_column));
    }
  | RETURN SEMICOLON {
      $$ = newnode(Return, NULL, @1.first_line, @1.first_column);
    }
  | RETURN Expr SEMICOLON {
      $$ = newnode(Return, NULL, @1.first_line, @1.first_column);
      addchild($$, $2);
    }
  | SEMICOLON { $$ = NULL; } // empty statement
  | MethodInvocation SEMICOLON { $$ = $1; }
  | Assignment SEMICOLON { $$ = $1; }
  | ParseArgs SEMICOLON { $$ = $1; }
  | PRINT LPAR Expr RPAR SEMICOLON {
      $$ = newnode(Print, NULL, @1.first_line, @1.first_column);
      addchild($$, $3);
    }
  | PRINT LPAR STRLIT RPAR SEMICOLON {
      $$ = newnode(Print, NULL, @1.first_line, @1.first_column);
      addchild($$, newnode(StrLit, $3, @3.first_line, @3.first_column));
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
      $$ = newnode(Call, NULL, @1.first_line, @1.first_column);
      addchild($$, newnode(Identifier, $1, @1.first_line, @1.first_column));
    }
  | IDENTIFIER LPAR ArgList RPAR {
      $$ = newnode(Call, NULL, @1.first_line, @1.first_column);
      addchild($$, newnode(Identifier, $1, @1.first_line, @1.first_column));
      addchildren($$, $3);
    }
  | IDENTIFIER LPAR error RPAR {
      $$ = newnode(Call, NULL, @1.first_line, @1.first_column);
      addchild($$, newnode(Identifier, $1, @1.first_line, @1.first_column));
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
      $$ = newnode(Assign, NULL, @2.first_line, @2.first_column);
      addchild($$, newnode(Identifier, $1, @1.first_line, @1.first_column));
      addchild($$, $3);
    }
  ;

ParseArgs
  : PARSEINT LPAR IDENTIFIER LSQ Expr RSQ RPAR {
      $$ = newnode(ParseArgs, NULL, @1.first_line, @1.first_column);
      addchild($$, newnode(Identifier, $3, @3.first_line, @3.first_column));
      addchild($$, $5);
    }
  | PARSEINT LPAR error RPAR {
      $$ = newnode(ParseArgs, NULL, @1.first_line, @1.first_column);
    }
  ;

Expr
  : Expr PLUS   Expr {
      $$ = newnode(Add, NULL, @2.first_line, @2.first_column);
      addchild($$, $1); // Expr1
      addchild($$, $3); // Expr2
    }
  | Expr MINUS  Expr {
      $$ = newnode(Sub, NULL, @2.first_line, @2.first_column);
      addchild($$, $1); // Expr1
      addchild($$, $3); // Expr2
    }
  | Expr STAR   Expr {
      $$ = newnode(Mul, NULL, @2.first_line, @2.first_column);
      addchild($$, $1); // Expr1
      addchild($$, $3); // Expr2
    }
  | Expr DIV    Expr {
      $$ = newnode(Div, NULL, @2.first_line, @2.first_column);
      addchild($$, $1); // Expr1
      addchild($$, $3); // Expr2
    }
  | Expr MOD    Expr {
      $$ = newnode(Mod, NULL, @2.first_line, @2.first_column);
      addchild($$, $1); // Expr1
      addchild($$, $3); // Expr2
    }
  | Expr AND    Expr {
      $$ = newnode(And, NULL, @2.first_line, @2.first_column);
      addchild($$, $1); // Expr1
      addchild($$, $3); // Expr2
    }
  | Expr OR     Expr {
      $$ = newnode(Or, NULL, @2.first_line, @2.first_column);
      addchild($$, $1); // Expr1
      addchild($$, $3); // Expr2
    }
  | Expr XOR    Expr {
      $$ = newnode(Xor, NULL, @2.first_line, @2.first_column);
      addchild($$, $1); // Expr1
      addchild($$, $3); // Expr2
    }
  | Expr LSHIFT Expr {
      $$ = newnode(Lshift, NULL, @2.first_line, @2.first_column);
      addchild($$, $1); // Expr1
      addchild($$, $3); // Expr2
    }
  | Expr RSHIFT Expr {
      $$ = newnode(Rshift, NULL, @2.first_line, @2.first_column);
      addchild($$, $1); // Expr1
      addchild($$, $3); // Expr2
    }
  | Expr EQ     Expr {
      $$ = newnode(Eq, NULL, @2.first_line, @2.first_column);
      addchild($$, $1); // Expr1
      addchild($$, $3); // Expr2
    }
  | Expr GE     Expr {
      $$ = newnode(Ge, NULL, @2.first_line, @2.first_column);
      addchild($$, $1); // Expr1
      addchild($$, $3); // Expr2
    }
  | Expr GT     Expr {
      $$ = newnode(Gt, NULL, @2.first_line, @2.first_column);
      addchild($$, $1); // Expr1
      addchild($$, $3); // Expr2
    }
  | Expr LE     Expr {
      $$ = newnode(Le, NULL, @2.first_line, @2.first_column);
      addchild($$, $1); // Expr1
      addchild($$, $3); // Expr2
    }
  | Expr LT     Expr {
      $$ = newnode(Lt, NULL, @2.first_line, @2.first_column);
      addchild($$, $1); // Expr1
      addchild($$, $3); // Expr2
    }
  | Expr NE     Expr {
      $$ = newnode(Ne, NULL, @2.first_line, @2.first_column);
      addchild($$, $1); // Expr1
      addchild($$, $3); // Expr2
    }
  | MINUS Expr            %prec UNARY {
      $$ = newnode(Minus, NULL, @1.first_line, @1.first_column);
      addchild($$, $2);
    }
  | PLUS  Expr            %prec UNARY {
      $$ = newnode(Plus, NULL, @1.first_line, @1.first_column);
      addchild($$, $2);
    }
  | NOT   Expr            %prec UNARY {
      $$ = newnode(Not, NULL, @1.first_line, @1.first_column);
      addchild($$, $2);
    }
  | LPAR Expr RPAR { $$ = $2; }
  | LPAR error RPAR {
      $$ = NULL;
    }
  | MethodInvocation { $$ = $1; }
  | Assignment { $$ = $1; }
  | ParseArgs { $$ = $1; }
  | IDENTIFIER {
      $$ = newnode(Identifier, $1, @1.first_line, @1.first_column);
    }
  | IDENTIFIER DOTLENGTH {
      $$ = newnode(Length, NULL, @2.first_line, @2.first_column);
      addchild($$, newnode(Identifier, $1, @1.first_line, @1.first_column));
    }
  | NATURAL {
      $$ = newnode(Natural, $1, @1.first_line, @1.first_column);
    }
  | DECIMAL {
      $$ = newnode(Decimal, $1, @1.first_line, @1.first_column);
    }
  | BOOLLIT {
      $$ = newnode(BoolLit, $1, @1.first_line, @1.first_column);
    }
  ;

%%

void yyerror(char *s) {
    syntax_errors++;
    int col = (yytext[0] == '\0') ? col_count : string_col;
    printf("Line %d, col %d: %s: %s\n", line_count, col, s, last_token_text[0] ? last_token_text : yytext);
    last_token_text[0] = '\0'; //reset
}
