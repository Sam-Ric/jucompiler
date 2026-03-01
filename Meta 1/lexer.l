/*
  João Tomás Correia Ferreira (2023217920)
  Samuel Marques Riça (2023206471)
*/

%{
    /* place here any C variables and definitions */
    int line_count = 1;
    int col_count = 1;
%}
digit       [0-9]
identifier  [a-zA-Z][a-zA-Z0-9]*
whitespace  [ \t]
escape_seq  \\[fnrt\\\"]
reserved    ("++"|"--"|"null"|"Integer"|"System")
%X COMMENT
%X STRING
%%
")"                     { printf("LPAR\n"); col_count += yyleng; }
"("                     { printf("RPAR\n"); col_count += yyleng; }
"{"                     { printf("LBRACE\n"); col_count += yyleng; }
"}"                     { printf("RBRACE\n"); col_count += yyleng; }
"["                     { printf("LSQ\n"); col_count += yyleng; }
"]"                     { printf("RSQ\n"); col_count += yyleng; }
"="                     { printf("ASSIGN\n"); col_count += yyleng; }
"=="                    { printf("EQ\n"); col_count += yyleng; }
">="                    { printf("GE\n"); col_count += yyleng; }
">"                     { printf("GT\n"); col_count += yyleng; }
"<"                     { printf("LT\n"); col_count += yyleng; }
"<="                    { printf("LE\n"); col_count += yyleng; }
"!="                    { printf("NE\n"); col_count += yyleng; }
"!"                     { printf("NOT\n"); col_count += yyleng; }
","                     { printf("COMMA\n"); col_count += yyleng; }
";"                     { printf("SEMICOLON\n"); col_count += yyleng; }
"*"                     { printf("STAR\n"); col_count += yyleng; }
"/"                     { printf("DIV\n"); col_count += yyleng; }
"+"                     { printf("PLUS\n"); col_count += yyleng; }
"-"                     { printf("MINUS\n"); col_count += yyleng; }
"%"                     { printf("MOD\n"); col_count += yyleng; }
"&&"                    { printf("AND\n"); col_count += yyleng; }
"||"                    { printf("OR\n"); col_count += yyleng; }
"->"                    { printf("ARROW\n"); col_count += yyleng; }
"<<"                    { printf("LSHIFT\n"); col_count += yyleng; }
">>"                    { printf("RSHIFT\n"); col_count += yyleng; }
"^"                     { printf("XOR\n"); col_count += yyleng; }
int                     { printf("INT\n"); col_count += yyleng; }
double                  { printf("DOUBLE\n"); col_count += yyleng; }
boolean                 { printf("BOOLEAN\n"); col_count += yyleng; }
string                  { printf("STRING\n"); col_count += yyleng; }
if                      { printf("IF\n"); col_count += yyleng; }
then                    { printf("THEN\n"); col_count += yyleng; }
else                    { printf("ELSE\n"); col_count += yyleng; }
{digit}+                { printf("NATURAL(%s)\n", yytext); col_count += yyleng; }
{digit}*"."{digit}+     { printf("DECIMAL(%s)\n", yytext); col_count += yyleng; }
true | false            { printf("BOOLLIT\n"); col_count += yyleng; }
class                   { printf("CLASS\n"); col_count += yyleng; }
public                  { printf("PUBLIC\n"); col_count += yyleng; }
return                  { printf("RETURN\n"); col_count += yyleng; }
static                  { printf("STATIC\n"); col_count += yyleng; }
void                    { printf("VOID\n"); col_count += yyleng; }
while                   { printf("WHILE\n"); col_count += yyleng; }
{reserved}              { printf("RESERVED\n"); col_count += yyleng; }
".length"               { printf("DOTLENGTH\n"); col_count += yyleng; }
"System.out.print"      { printf("PRINT\n"); col_count += yyleng; }
"Integer.parseInt"      { printf("PARSEINT\n"); col_count += yyleng; }
{identifier}            { printf("IDENTIFIER(%s)\n", yytext); col_count += yyleng; }
"\n"                    { line_count++; col_count = 1; }
{whitespace}            { col_count += yyleng; }

\"                      { BEGIN(STRING); printf("STRLIT(\""); col_count++; }
<STRING>{escape_seq}+   { ECHO; col_count += yyleng; }
<STRING>{identifier}+   { ECHO; col_count += yyleng; }
<STRING>\"              { col_count += 1; printf("\")"); BEGIN(INITIAL); }
<STRING>\\.             { printf("Line %d, column %d: invalid escape sequence (%s)\n", line_count, col_count, yytext); BEGIN(INITIAL); }

"/*"                    { BEGIN(COMMENT); }
<COMMENT>.              { col_count += yyleng; }
<COMMENT>"\n"           { line_count++; col_count = 1; }
<COMMENT>"*/"           { col_count += 2; BEGIN(INITIAL); }

.                       { printf("Line %d, column %d: unrecognized character (%s)\n", line_count, col_count, yytext); }
%%
extern int yylex();
int main() {
    yylex();    /* run the lexical analysis automaton */
    return 0;
}
int yywrap() {  /* called on EOF, return 1 to terminate */
    return 1;
}
