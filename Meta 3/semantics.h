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


#endif /* SEMANTIC_H */
