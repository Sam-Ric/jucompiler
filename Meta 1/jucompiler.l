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
%X COMMENT
%X STRING
%%
")"                     { printf(")\n"); col_count += yyleng; }
"("                     { printf("(\n"); col_count += yyleng; }
"="                     { printf("=\n"); col_count += yyleng; }
","                     { printf(",\n"); col_count += yyleng; }
"*"                     { printf("*\n"); col_count += yyleng; }
"/"                     { printf("/\n"); col_count += yyleng; }
"+"                     { printf("+\n"); col_count += yyleng; }
"-"                     { printf("-\n"); col_count += yyleng; }
integer                 { printf("INTEGER\n"); col_count += yyleng; }
double                  { printf("DOUBLE\n"); col_count += yyleng; }
if                      { printf("IF\n"); col_count += yyleng; }
then                    { printf("THEN\n"); col_count += yyleng; }
else                    { printf("ELSE\n"); col_count += yyleng; }
{digit}+                { printf("NATURAL(%s)\n", yytext); col_count += yyleng; }
{digit}*"."{digit}+     { printf("DECIMAL(%s)\n", yytext); col_count += yyleng; }
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
