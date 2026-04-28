// This file is part of the Petit compiler.
// SPDX-License-Identifier: BSD-3-Clause

#ifndef _AST_H
#define _AST_H

// the order of the enum and the #define must precisely match
enum category {
    Program, 
    FieldDecl, VarDecl, MethodDecl, MethodHeader, MethodParams, ParamDecl, MethodBody,
    Block, If, While, Return, Call, Print, ParseArgs, Assign,
    Or, And, Eq, Ne, Lt, Gt, Le, Ge, Add, Sub, Mul, Div, Mod, 
    Lshift, Rshift, Xor, Not, Minus, Plus, Length,
    Bool, BoolLit, Double, Decimal, Identifier, Int, Natural, StrLit, StringArray, Void
};

#define names { \
    "Program", \
    "FieldDecl", "VarDecl", "MethodDecl", "MethodHeader", "MethodParams", "ParamDecl", "MethodBody", \
    "Block", "If", "While", "Return", "Call", "Print", "ParseArgs", "Assign", \
    "Or", "And", "Eq", "Ne", "Lt", "Gt", "Le", "Ge", "Add", "Sub", "Mul", "Div", "Mod", \
    "Lshift", "Rshift", "Xor", "Not", "Minus", "Plus", "Length", \
    "Bool", "BoolLit", "Double", "Decimal", "Identifier", "Int", "Natural", "StrLit", "StringArray", "Void" \
}

enum type {
    type_int,
    type_double,
    type_boolean,
    type_void,
    type_string_array,
    type_undef,
    type_none
};

struct node {
    enum category category;
    char *token;
    int token_line, token_column;
    enum type type;
    struct node_list *children;
};

struct node_list {
    struct node *node;
    struct node_list *next;
};

struct node *newnode(enum category category, char *token, int line, int col);
void addchild(struct node *parent, struct node *child);
struct node *getchild(struct node *parent, int position);
int countchildren(struct node *node);
struct node_list *newlist();
void append(struct node_list *list, struct node *node);
void addchildren(struct node *node, struct node_list *list);
void show(struct node *root, int depth);
void freenode(struct node *node);

#endif