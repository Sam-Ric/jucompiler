/*
  João Tomás Correia Ferreira (2023217920)
  Samuel Marques Riça (2023206471)
*/

%{
    /* place here any C variables and definitions */
    #include <string.h>
    int line_count = 1;
    int col_count = 1;
    char string_buffer[1024];

    int print = 0;
    void handle_token(char* token, int yyleng) {
        col_count += yyleng;
        if (print)
            printf("%s\n", token);
    }

%}
digit       [0-9]
identifier  [a-zA-Z][a-zA-Z0-9]*
whitespace  [ \t]
escape_seq  \\[fnrt\\\"]
reserved    ("++"|"--"|"null"|"Integer"|"System")
%X COMMENT1
%X COMMENT2
%X STRING
%%
"("                     { handle_token("LPAR", yyleng); }
")"                     { handle_token("RPAR", yyleng); }
"{"                     { handle_token("LBRACE", yyleng); }
"}"                     { handle_token("RBRACE", yyleng); }
"["                     { handle_token("LSQ", yyleng); }
"]"                     { handle_token("RSQ", yyleng); }

"=="                    { handle_token("EQ", yyleng); }
"="                     { handle_token("ASSIGN", yyleng); }

">="                    { handle_token("GE", yyleng); }
">"                     { handle_token("GT", yyleng); }
"<="                    { handle_token("LE", yyleng); }
"<"                     { handle_token("LT", yyleng); }

"!="                    { handle_token("NE", yyleng); }
"!"                     { handle_token("NOT", yyleng); }

","                     { handle_token("COMMA", yyleng); }
";"                     { handle_token("SEMICOLON", yyleng); }

"*"                     { handle_token("STAR", yyleng); }
"/"                     { handle_token("DIV", yyleng); }
"+"                     { handle_token("PLUS", yyleng); }
"-"                     { handle_token("MINUS", yyleng); }
"%"                     { handle_token("MOD", yyleng); }

"&&"                    { handle_token("AND", yyleng); }
"||"                    { handle_token("OR", yyleng); }
"->"                    { handle_token("ARROW", yyleng); }
"<<"                    { handle_token("LSHIFT", yyleng); }
">>"                    { handle_token("RSHIFT", yyleng); }
"^"                     { handle_token("XOR", yyleng); }

int                     { handle_token("INT", yyleng); }
double                  { handle_token("DOUBLE", yyleng); }
boolean                 { handle_token("BOOLEAN", yyleng); }
String                  { handle_token("STRING", yyleng); }

if                      { handle_token("IF", yyleng); }
then                    { handle_token("THEN", yyleng); }
else                    { handle_token("ELSE", yyleng); }

true|false              { handle_token("BOOLLIT", yyleng); }

class                   { handle_token("CLASS", yyleng); }
public                  { handle_token("PUBLIC", yyleng); }
return                  { handle_token("RETURN", yyleng); }
static                  { handle_token("STATIC", yyleng); }
void                    { handle_token("VOID", yyleng); }
while                   { handle_token("WHILE", yyleng); }

{reserved}              { handle_token("RESERVED", yyleng); }
".length"               { handle_token("DOTLENGTH", yyleng); }
"System.out.print"      { handle_token("PRINT", yyleng); }
"Integer.parseInt"      { handle_token("PARSEINT", yyleng); }

{digit}+                { printf("NATURAL(%s)\n", yytext); col_count += yyleng; }
{digit}*"."{digit}+     { printf("DECIMAL(%s)\n", yytext); col_count += yyleng; }
{identifier}            { if (print) printf("IDENTIFIER(%s)\n", yytext); col_count += yyleng; }
"\n"                    { line_count++; col_count = 1; }
{whitespace}            { col_count += yyleng; }

\"                      { BEGIN(STRING); string_buffer[0] = '\0'; strcat(string_buffer, "STRLIT(\""); col_count += yyleng; }
<STRING>\"              { col_count += yyleng; strcat(string_buffer,"\")"); printf("%s\n", string_buffer); BEGIN(INITIAL); }
<STRING>{escape_seq}    { strcat(string_buffer, yytext); col_count += yyleng; }
<STRING>\\.             { printf("Line %d, column %d: invalid escape sequence (%s)\n", line_count, col_count, yytext); BEGIN(INITIAL); }
<STRING>\n              { printf("Line %d, column %d: invalid string (newline in string)\n", line_count, col_count); }
<STRING>\r              { printf("Line %d, column %d: invalid string (carriage return in string)\n", line_count, col_count); }
<STRING><<EOF>>         { printf("Line %d, column %d: invalid string (EOF in string)\n", line_count, col_count); }
<STRING>[^\\\"\n\r]+	{ col_count += yyleng; strcat(string_buffer, yytext); }

"//"                    { BEGIN(COMMENT1); }
<COMMENT1>"\n"          { line_count++; col_count = 1; BEGIN(INITIAL); }

"/*"                     { BEGIN(COMMENT2); }
<COMMENT2>"*/"           { col_count += 2; BEGIN(INITIAL); }
<COMMENT2>"\n"           { line_count++; col_count = 1; }
<COMMENT2>.              { col_count += yyleng; }

.                       { printf("Line %d, column %d: unrecognized character (%s)\n", line_count, col_count, yytext); }
%%
extern int yylex();
int main(int argc, char** argv) {
    if (argc > 2) {
        printf("Invalid option\n");
        return 1;
    }

    // Execution options
    if (argc == 2) {
        // -- Print errors and tokens
        if (strcmp(argv[1], "-l") == 0)
            print = 1;
        // -- Print errors only
        else if (strcmp(argv[1], "-e1") == 0) {
            // ...
        }
        else {
            printf("Invalid option\n");
            return 1;
        }
    }

    yylex();    /* run the lexical analysis automaton */
    return 0;
}
int yywrap() {  /* called on EOF, return 1 to terminate */
    return 1;
}
