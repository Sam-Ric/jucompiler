/*
 João Tomás Correia Ferreira (2023217920)
 Samuel Marques Riça (2023206471)
*/

#ifndef _CODEGEN_H
#define _CODEGEN_H

#include "ast.h"

/* Ponto de entrada principal para a geração de código LLVM IR */
void codegen_program(struct node *program);

#endif /* _CODEGEN_H */