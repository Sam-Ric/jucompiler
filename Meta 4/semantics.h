/*
João Tomás Correia Ferreira (2023217920)
Samuel Marques Riça (2023206471)
*/

#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "ast.h"

/*
 * Run all three semantic passes (symbol-table construction + type checking).
 * Errors are printed to stdout as they are encountered.
 * Returns the total number of semantic errors found.
 */
int semantic_analysis(struct node *program);

/*
 * Print all symbol tables to stdout.
 * Format: "===== Class/Method <sig> Symbol Table ====="
 * Must be called after semantic_analysis().
 */
void print_tables(void);

/*
 * Print the annotated AST to stdout.
 * Expression nodes are annotated with " - <type>".
 * Must be called after semantic_analysis().
 */
void show_annotated(struct node *node, int depth);

/* Utility exposed for use in jucompiler.y / code generation */
const char *type_to_string(enum type t);
enum type   category_to_type(enum category c);

/* =========================================================================
   DATA STRUCTURES
   ========================================================================= */

typedef struct param_entry
{
    char *type_str;
    struct param_entry *next;
} param_entry;

typedef struct symbol_entry
{
    char *name;
    char *type_str; /* "int", "double", … */
    int is_method;
    int is_param;        /* 1 for formal parameters */
    param_entry *params; /* only set for methods    */
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
    symbol_entry *symbols; /* fields + method headers */
    method_table *methods;
} class_table;

extern class_table *gtable;

#endif /* SEMANTIC_H */
