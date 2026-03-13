/*
  João Tomás Correia Ferreira (2023217920)
  Samuel Marques Riça (2023206471)
*/

%{
    /* place here any C variables and definitions */
    #include <string.h>
    int line_count = 1;
    int col_count = 1;
    int comment_line, comment_col;
    char string_buffer[1024];
    int string_line, string_col, string_error;

    int print = 0;
    void handle_token(char* token, int yyleng) {
        col_count += yyleng;
        if (print)
            printf("%s\n", token);
    }

%}
natural     0|[1-9]([0-9_]*[0-9])?
integer     [0-9]([0-9_]*[0-9])?
exponent    [eE][+-]?{integer}
decimal     ({integer}"."{integer}?{exponent}?)|("."{integer}{exponent}?)|({integer}{exponent})
identifier  [a-zA-Z_$][a-zA-Z0-9_$]*
whitespace  [ \t]
escape_seq  \\[fnrt\\\"]
reserved    "++"|"--"|"null"|"Integer"|"System"|"abstract"|"assert"|"break"|"byte"|"case"|"catch"|"char"|"const"|"continue"|"default"|"do"|"enum"|"extends"|"final"|"finally"|"float"|"for"|"goto"|"implements"|"import"|"instanceof"|"interface"|"long"|"native"|"new"|"package"|"private"|"protected"|"short"|"strictfp"|"super"|"switch"|"synchronized"|"this"|"throw"|"throws"|"transient"|"try"|"volatile"
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

"//"                    { BEGIN(COMMENT1); }
"/*"                    { BEGIN(COMMENT2); comment_line = line_count; comment_col = col_count; col_count += 2; }

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
boolean                 { handle_token("BOOL", yyleng); }
String                  { handle_token("STRING", yyleng); }

if                      { handle_token("IF", yyleng); }
else                    { handle_token("ELSE", yyleng); }

true|false              { printf("BOOLLIT(%s)\n", yytext); col_count += yyleng; }

class                   { handle_token("CLASS", yyleng); }
public                  { handle_token("PUBLIC", yyleng); }
return                  { handle_token("RETURN", yyleng); }
static                  { handle_token("STATIC", yyleng); }
void                    { handle_token("VOID", yyleng); }
while                   { handle_token("WHILE", yyleng); }

{reserved}              { if (print) printf("RESERVED(%s)\n", yytext); col_count += yyleng; }
".length"               { handle_token("DOTLENGTH", yyleng); }
"System.out.print"      { handle_token("PRINT", yyleng); }
"Integer.parseInt"      { handle_token("PARSEINT", yyleng); }

{natural}               { if (print) printf("NATURAL(%s)\n", yytext); col_count += yyleng; }
{decimal}               { if (print) printf("DECIMAL(%s)\n", yytext); col_count += yyleng; }
{identifier}            { if (print) printf("IDENTIFIER(%s)\n", yytext); col_count += yyleng; }
"\r\n"                  { line_count++; col_count = 1; }
"\r"                    { line_count++; col_count = 1; }
"\n"                    { line_count++; col_count = 1; }
{whitespace}            { col_count += yyleng; }

\"                      { BEGIN(STRING); string_buffer[0] = '\0'; strcat(string_buffer, "STRLIT(\""); string_line = line_count; string_col = col_count; col_count += yyleng; string_error = 0; }
<STRING>\"              { col_count += yyleng; if (!string_error) {strcat(string_buffer,"\")"); if (print) printf("%s\n", string_buffer);} BEGIN(INITIAL); }
<STRING>\\\r\n          { printf("Line %d, col %d: invalid escape sequence (\\)\n", line_count, col_count); printf("Line %d, col %d: unterminated string literal\n", string_line, string_col); line_count++; col_count = 1; string_error = 1; BEGIN(INITIAL); }
<STRING>\\\n            { printf("Line %d, col %d: invalid escape sequence (\\)\n", line_count, col_count); printf("Line %d, col %d: unterminated string literal\n", string_line, string_col); line_count++; col_count = 1; string_error = 1; BEGIN(INITIAL); }
<STRING>{escape_seq}    { strncat(string_buffer, yytext, sizeof(string_buffer)-strlen(string_buffer)-1); col_count += yyleng; }
<STRING>\\.             { printf("Line %d, col %d: invalid escape sequence (%s)\n", line_count, col_count, yytext); col_count += yyleng; string_error = 1;}
<STRING>"\r\n"          { printf("Line %d, col %d: unterminated string literal\n", string_line, string_col); line_count++; col_count = 1; BEGIN(INITIAL); }
<STRING>\n              { printf("Line %d, col %d: unterminated string literal\n", string_line, string_col); line_count++; col_count = 1; BEGIN(INITIAL); }
<STRING>\r              { printf("Line %d, col %d: unterminated string literal\n", string_line, string_col); col_count++; BEGIN(INITIAL);}
<STRING><<EOF>>         { printf("Line %d, col %d: unterminated string literal\n", string_line, string_col); yyterminate(); }
<STRING>[^\\"\n\r]+	{ col_count += yyleng; strncat(string_buffer, yytext, sizeof(string_buffer)-strlen(string_buffer)-1); }

<COMMENT1>"\r\n"        { line_count++; col_count = 1; BEGIN(INITIAL); }
<COMMENT1>\r            { line_count++; col_count = 1; BEGIN(INITIAL); }
<COMMENT1>\n            { line_count++; col_count = 1; BEGIN(INITIAL); }
<COMMENT1><<EOF>>       { yyterminate(); }
<COMMENT1>.             { ; }

<COMMENT2>"*/"           { col_count += 2; BEGIN(INITIAL); }
<COMMENT2>"\r\n"         { line_count++; col_count = 1; }
<COMMENT2>\r             { line_count++; col_count = 1; }
<COMMENT2>\n             { line_count++; col_count = 1; }
<COMMENT2><<EOF>>        { printf("Line %d, col %d: unterminated comment\n", comment_line, comment_col); yyterminate(); }
<COMMENT2>.              { col_count++; }

.                       { printf("Line %d, col %d: illegal character (%s)\n", line_count, col_count, yytext); col_count++; }
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
