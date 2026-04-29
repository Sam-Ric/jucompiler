/*
  João Tomás Correia Ferreira (2023217920)
  Samuel Marques Riça (2023206471)
*/

%{
    /* place here any C variables and definitions */
    #include "y.tab.h"
    #include "ast.h"
    #include "semantics.h"
    #include <string.h>

    extern int yyparse();
    extern int syntax_errors;
    extern struct node *root;
    
    int line_count = 1;
    int col_count = 1;
    int comment_line, comment_col;
    char string_buffer[1024];
    char string_content[1024]; // Second buffer to store only the string content without the STRLIT.
    char last_token_text[1024];
    int string_line, string_col, string_error;

    int print = 0;
    void handle_token(char* token, int yyleng) {
        yylloc.first_line = line_count;
        yylloc.first_column = col_count;

        string_col = col_count;
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
whitespace  [ \t\f]
escape_seq  \\[fnrt\\\"]
reserved    "++"|"--"|"null"|"Integer"|"System"|"abstract"|"assert"|"break"|"byte"|"case"|"catch"|"char"|"const"|"continue"|"default"|"do"|"enum"|"extends"|"final"|"finally"|"float"|"for"|"goto"|"implements"|"import"|"instanceof"|"interface"|"long"|"native"|"new"|"package"|"private"|"protected"|"short"|"strictfp"|"super"|"switch"|"synchronized"|"this"|"throw"|"throws"|"transient"|"try"|"volatile"
%X COMMENT1
%X COMMENT2
%X STR

%%
"("                     { handle_token("LPAR", yyleng); return LPAR; }
")"                     { handle_token("RPAR", yyleng); return RPAR; }
"{"                     { handle_token("LBRACE", yyleng); return LBRACE; }
"}"                     { handle_token("RBRACE", yyleng); return RBRACE; }
"["                     { handle_token("LSQ", yyleng); return LSQ; }
"]"                     { handle_token("RSQ", yyleng); return RSQ; }

"=="                    { handle_token("EQ", yyleng); return EQ; }
"="                     { handle_token("ASSIGN", yyleng); return ASSIGN; }

">="                    { handle_token("GE", yyleng); return GE; }
">"                     { handle_token("GT", yyleng); return GT; }
"<="                    { handle_token("LE", yyleng); return LE; }
"<"                     { handle_token("LT", yyleng); return LT; }

"!="                    { handle_token("NE", yyleng); return NE; }
"!"                     { handle_token("NOT", yyleng); return NOT; }

","                     { handle_token("COMMA", yyleng); return COMMA; }
";"                     { handle_token("SEMICOLON", yyleng); return SEMICOLON; }

"//"                    { BEGIN(COMMENT1); }
"/*"                    { BEGIN(COMMENT2); comment_line = line_count; comment_col = col_count; col_count += 2; }

"*"                     { handle_token("STAR", yyleng); return STAR; }
"/"                     { handle_token("DIV", yyleng); return DIV; }
"+"                     { handle_token("PLUS", yyleng); return PLUS; }
"-"                     { handle_token("MINUS", yyleng); return MINUS; }
"%"                     { handle_token("MOD", yyleng); return MOD; }

"&&"                    { handle_token("AND", yyleng); return AND; }
"||"                    { handle_token("OR", yyleng); return OR; }
"->"                    { handle_token("ARROW", yyleng); return ARROW; }
"<<"                    { handle_token("LSHIFT", yyleng); return LSHIFT; }
">>"                    { handle_token("RSHIFT", yyleng); return RSHIFT; }
"^"                     { handle_token("XOR", yyleng); return XOR; }

int                     { handle_token("INT", yyleng); return INT; }
double                  { handle_token("DOUBLE", yyleng); return DOUBLE; }
boolean                 { handle_token("BOOL", yyleng); return BOOL; }
String                  { handle_token("STRING", yyleng); return STRING; }

if                      { handle_token("IF", yyleng); return IF; }
else                    { handle_token("ELSE", yyleng); return ELSE; }

true|false              { string_col = col_count; if (print) printf("BOOLLIT(%s)\n", yytext); yylloc.first_line = line_count;  yylloc.first_column = col_count; yylval.val = strdup(yytext); col_count += yyleng; return BOOLLIT; }

class                   { handle_token("CLASS", yyleng); return CLASS; }
public                  { handle_token("PUBLIC", yyleng); return PUBLIC; }
return                  { handle_token("RETURN", yyleng); return RETURN; }
static                  { handle_token("STATIC", yyleng); return STATIC; }
void                    { handle_token("VOID", yyleng); return VOID; }
while                   { handle_token("WHILE", yyleng); return WHILE; }

{reserved}              { string_col = col_count; if (print) printf("RESERVED(%s)\n", yytext); yylloc.first_line = line_count;  yylloc.first_column = col_count; col_count += yyleng; return RESERVED;}
".length"               { handle_token("DOTLENGTH", yyleng); return DOTLENGTH; }
"System.out.print"      { handle_token("PRINT", yyleng); return PRINT; }
"Integer.parseInt"      { handle_token("PARSEINT", yyleng); return PARSEINT; }

{natural}               { string_col = col_count; if (print) printf("NATURAL(%s)\n", yytext); yylloc.first_line = line_count;  yylloc.first_column = col_count; yylval.val = strdup(yytext); col_count += yyleng; return NATURAL; }
{decimal}               { string_col = col_count; if (print) printf("DECIMAL(%s)\n", yytext); yylloc.first_line = line_count;  yylloc.first_column = col_count; yylval.val = strdup(yytext); col_count += yyleng; return DECIMAL; }
{identifier}            { string_col = col_count; if (print) printf("IDENTIFIER(%s)\n", yytext); yylloc.first_line = line_count;  yylloc.first_column = col_count; yylval.val = strdup(yytext); col_count += yyleng; return IDENTIFIER; }
"\r\n"                  { line_count++; col_count = 1; }
"\r"                    { line_count++; col_count = 1; }
"\n"                    { line_count++; col_count = 1; }
{whitespace}            { col_count += yyleng; }

\"                      { BEGIN(STR); string_buffer[0] = '\0'; strcat(string_buffer, "STRLIT(\""); string_content[0] = '\0'; strcat(string_content, "\""); string_line = line_count; string_col = col_count; col_count += yyleng; string_error = 0; }
<STR>\" {
        col_count += yyleng;
        if (!string_error) {
            strcat(string_buffer,"\")");
            strcat(string_content,"\"");
            if (print)
                printf("%s\n", string_buffer);
            yylval.val = strdup(string_content);
            col_count = string_col;
        } BEGIN(INITIAL);
        if (!string_error) {
            strncpy(last_token_text, string_content, sizeof(last_token_text)-1);
            return STRLIT;
        }
    }
<STR>\\\r\n {
        printf("Line %d, col %d: invalid escape sequence (\\)\n", line_count, col_count);
        printf("Line %d, col %d: unterminated string literal\n", string_line, string_col);
        line_count++;
        col_count = 1;
        string_error = 1;
        BEGIN(INITIAL);
    }
<STR>\\\r {
        printf("Line %d, col %d: invalid escape sequence (\\)\n", line_count, col_count);
        printf("Line %d, col %d: unterminated string literal\n", string_line, string_col);
        line_count++;
        col_count = 1;
        string_error = 1;
        BEGIN(INITIAL);
    }
<STR>\\\n {
        printf("Line %d, col %d: invalid escape sequence (\\)\n", line_count, col_count);
        printf("Line %d, col %d: unterminated string literal\n", string_line, string_col);
        line_count++;
        col_count = 1;
        string_error = 1;
        BEGIN(INITIAL);
    }
<STR>{escape_seq} {
        strncat(string_buffer, yytext, sizeof(string_buffer)-strlen(string_buffer)-1);
        strncat(string_content, yytext, sizeof(string_content)-strlen(string_content)-1);
        col_count += yyleng;
    }
<STR>\\. {
        printf("Line %d, col %d: invalid escape sequence (%s)\n", line_count, col_count, yytext);
        col_count += yyleng;
        string_error = 1;
    }
<STR>"\r\n" {
        printf("Line %d, col %d: unterminated string literal\n", string_line, string_col);
        line_count++;
        col_count = 1;
        BEGIN(INITIAL);
    }
<STR>\n {
        printf("Line %d, col %d: unterminated string literal\n", string_line, string_col);
        line_count++;
        col_count = 1;
        BEGIN(INITIAL);
    }
<STR>\r {
        printf("Line %d, col %d: unterminated string literal\n", string_line, string_col);
        line_count++;
        col_count = 1;
        BEGIN(INITIAL);
    }
<STR><<EOF>> {
        printf("Line %d, col %d: unterminated string literal\n", string_line, string_col);
        yyterminate();
    }
<STR>[^\\"\n\r]+ {
        col_count += yyleng;
        strncat(string_buffer, yytext, sizeof(string_buffer)-strlen(string_buffer)-1);
        strncat(string_content, yytext, sizeof(string_content)-strlen(string_content)-1);
    }

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
    int mode = 0; // 0: Default, 1: -l, 2: -e1, 3: -t, 4: -e2

    if (argc > 1) {
        if (strcmp(argv[1], "-l") == 0) mode = 1;
        else if (strcmp(argv[1], "-e1") == 0) mode = 2;
        else if (strcmp(argv[1], "-t") == 0) mode = 3;
        else if (strcmp(argv[1], "-e2") == 0) mode = 4;
        else if (strcmp(argv[1], "-s") == 0) mode = 5;
        else if (strcmp(argv[1], "-e3") == 0) mode = 6;
    }

    if (mode == 1 || mode == 2) {
        // Only lexical analysis
        if (mode == 1) 
            print = 1; // Activates tokens printing
        while (yylex()); 
        return 0; // Ends without calling the parser
    }

    // Invocation of the parser
    yyparse(); 

    if (mode == 3 && !syntax_errors) show(root, 0);
        else if (!syntax_errors && (mode == 5 || mode == 6 || mode == 0)) {
            semantic_analysis(root); // build tables, analyse types
            if (mode == 5) { 
                print_tables();
                printf("\n");
                show_annotated(root, 0); }
            // mode 6: only errors
        }
    
    // Memory free.
    if (root) freenode(root);

    return 0;
}

int yywrap() {  /* called on EOF, return 1 to terminate */
    return 1;
}
