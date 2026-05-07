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


#endif /* SEMANTIC_H */
