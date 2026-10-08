#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "ast.h"

/* Analiza el AST entero. Devuelve 0 si OK, cantidad de errores si falla. */
int semantic_analyze(ASTNode *root);

#endif