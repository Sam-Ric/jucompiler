#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "semantics.h"

int semantic_errors = 0;

struct symbol_list *symbol_table;

char* type_name(enum type t) {
    switch (t) {
        case 0: // type_int
            return "int";
            break;
        case 1: // type_double
            return "double";
            break;
        case 2: // type_boolean
            return "boolean";
            break;
        case 3: // type_void
            return "void";
            break;
        case 4: // type_string_array
            return "String[]";
            break;
        case 5: // type_undef
            return "undefined";
            break;
        case 6: // type_none
            return "none";
            break;
        default:
            break;
    }
    return "";
}

void check_function(struct node *function) {
    if (function->category != MethodDecl) {
        return; // Not a function/method, so we skip it.
    }
    struct node *method_header = getchild(function, 0);
    struct node *id = getchild(method_header, 1);
    if(search_symbol(symbol_table, id->token) == NULL) {
        insert_symbol(symbol_table, id->token, type_none, function);
    } else {
        printf("Identifier %s already declared\n", id->token);
        semantic_errors++;
    }
    //check_parameters(getchild(function, 1));
    //check_expression(getchild(function, 2));
}

// semantic analysis begins here, with the AST root node
int check_program(struct node *program) {
    symbol_table = (struct symbol_list *) malloc(sizeof(struct symbol_list));
    symbol_table->next = NULL;
    struct node_list *child = program->children;
    while(child != NULL && (child = child->next) != NULL)
        check_function(child->node);
    return semantic_errors;
}

// insert a new symbol in the list, unless it is already there
struct symbol_list *insert_symbol(struct symbol_list *table, char *identifier, enum type type, struct node *node) {
    if(search_symbol(table, identifier) != NULL)
        return NULL;       /* return NULL if symbol is already inserted */
    struct symbol_list *new = (struct symbol_list *) malloc(sizeof(struct symbol_list));
    new->identifier = strdup(identifier);
    new->type = type;
    new->node = node;
    new->next = NULL;
    struct symbol_list *symbol = table;
    while(symbol->next != NULL)
        symbol = symbol->next;
    symbol->next = new;    /* insert new symbol at the tail of the list */
    return new;
}

// look up a symbol by its identifier
struct symbol_list *search_symbol(struct symbol_list *table, char *identifier) {
    struct symbol_list *symbol;
    for(symbol = table->next; symbol != NULL; symbol = symbol->next)
        if(strcmp(symbol->identifier, identifier) == 0)
            return symbol;
    return NULL;
}

void show_symbol_table() {
    struct symbol_list *symbol;
    for(symbol = symbol_table->next; symbol != NULL; symbol = symbol->next)
        printf("Symbol %s : %s\n", symbol->identifier, type_name(symbol->type));
}
