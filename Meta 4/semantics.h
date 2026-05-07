/*
João Tomás Correia Ferreira (2023217920)
Samuel Marques Riça (2023206471)
*/

#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "ast.h"

// Função pra correr a analise semantica toda
int semantic_analysis(struct node *program);

// Printa as tabelas de simbolo pro stdout
void print_tables(void);

// Dá print à AST com as declaraçoes dos tipos e etc...
void show_annotated(struct node *node, int depth);

// Pode ser util no codegen
const char *type_to_string(enum type t);
enum type   category_to_type(enum category c);

// Estruturas de dados nesta meta tiverem de passar para o header file para serem usadas no codegen.
typedef struct param_entry
{
    char *type_str;
    struct param_entry *next;
} param_entry;

typedef struct symbol_entry
{
    char *name;
    char *type_str; // int, double, etc
    int is_method;
    int is_param;        // 1 se for parametros
    param_entry *params; // só existe nos metodos
    int line, col;
    struct symbol_entry *next;
} symbol_entry;

typedef struct method_table
{
    char *name;            /* e.g. "factorial"      */
    char *signature;       /* e.g. "factorial(int)" */
    symbol_entry *symbols; /* return, params, locals */
    struct method_table *next;
} method_table;

typedef struct
{
    char *name;
    symbol_entry *symbols; // variaveis e declarações de métodos
    method_table *methods; // métodos em si. Contem a informação de cada método
} class_table;

// GLOBAL STATE
extern class_table *gtable;

#endif /* SEMANTIC_H */
