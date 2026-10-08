#include "semantic.h"
#include "symbol_table.h"
#include <stdio.h>
#include <string.h>

static int errors = 0;

static const char *infer_type(ASTNode *init) {
    if (!init) return "unknown";
    switch (init->type) {
        case INT_LITERAL:   return "Number";
        case FLOAT_LITERAL: return "Number";
        case STR_LITERAL:   return "Text";
        case BOOL_LITERAL:  return "Boolean";
        default:            return "unknown";
    }
}

static int type_matches(const char *declared, const char *actual) {
    if (strcmp(declared, "inferred") == 0) return 1;
    if (strcmp(declared, actual) == 0) return 1;
    /* Angle y Distance son Number por debajo */
    if ((strcmp(declared, "Angle") == 0 || strcmp(declared, "Distance") == 0)
        && strcmp(actual, "Number") == 0) return 1;
    return 0;
}

static void check_declaration(ASTNode *node, const char *kind) {
    const char *name = node->varDeclaration.identifier;
    const char *declared_type = node->varDeclaration.dataType;
    const char *inferred = infer_type(node->varDeclaration.init);

    /* Chequeo 1: redeclaración */
    if (symtab_lookup(name) == NULL) {
        /* No está en la tabla: el parser debería haberlo insertado.
           Caso raro, pero por completitud lo reportamos. */
    }

    /* Chequeo 2: coincidencia de tipo */
    if (!type_matches(declared_type, inferred)) {
        fprintf(stderr,
                "[Semantic Error] %s '%s': declared type '%s' does not match value type '%s'\n",
                kind, name, declared_type, inferred);
        errors++;
    }
}

int semantic_analyze(ASTNode *root) {
    errors = 0;

    /* Para detectar redeclaraciones recorremos el AST en orden
       y vamos marcando cuáles ya vimos. Usamos una tabla auxiliar
       (reaprovechamos lookup contra lo ya insertado por el parser:
       si aparece dos veces en el AST = redeclaración). */

    /* Recorrido 1: chequeos de tipo */
    ASTNode *cur = root;
    while (cur != NULL) {
        if (cur->type == VAR_DECL) {
            check_declaration(cur, "Variable");
        } else if (cur->type == CONST_DECL) {
            check_declaration(cur, "Constant");
        }
        cur = cur->next;
    }

    /* Recorrido 2: detectar redeclaraciones comparando nombres en el AST */
    cur = root;
    while (cur != NULL) {
        if (cur->type == VAR_DECL || cur->type == CONST_DECL) {
            const char *name1 = cur->varDeclaration.identifier;
            ASTNode *other = cur->next;
            while (other != NULL) {
                if ((other->type == VAR_DECL || other->type == CONST_DECL) &&
                    strcmp(other->varDeclaration.identifier, name1) == 0) {
                    fprintf(stderr,
                            "[Semantic Error] Identifier '%s' is declared more than once\n",
                            name1);
                    errors++;
                }
                other = other->next;
            }
        }
        cur = cur->next;
    }

    return errors;
}