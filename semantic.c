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

/* NUEVO (arrays): detecta si un tipo es un pack, o sea si termina en "[]". */
static int is_pack_type(const char *t) {
    size_t n = strlen(t);
    return n >= 2 && t[n - 2] == '[' && t[n - 1] == ']';
}

/* NUEVO (arrays): valida que cada elemento cuadre con el tipo del pack. */
static void check_pack_declaration(ASTNode *node, const char *kind) {
    const char *name = node->varDeclaration.identifier;
    const char *declared = node->varDeclaration.dataType; /* ej: "Number[]" */
    ASTNode *init = node->varDeclaration.init;

    /* El tipo del elemento es lo declarado sin el "[]" final */
    char element_type[64];
    size_t len = strlen(declared) - 2;
    if (len >= sizeof(element_type)) len = sizeof(element_type) - 1;
    memcpy(element_type, declared, len);
    element_type[len] = '\0';

    if (!init || init->type != ARRAY_LITERAL) {
        fprintf(stderr,
                "[Semantic Error] %s '%s': se esperaba un literal de array para el tipo '%s'\n",
                kind, name, declared);
        errors++;
        return;
    }

    for (ASTNode *el = init->arrayLit.elements; el != NULL; el = el->next) {
        const char *el_type = infer_type(el);
        if (!type_matches(element_type, el_type)) {
            fprintf(stderr,
                    "[Semantic Error] %s '%s': elemento de tipo '%s' no coincide con '%s'\n",
                    kind, name, el_type, element_type);
            errors++;
        }
    }
}

static void check_declaration(ASTNode *node, const char *kind) {
    const char *name = node->varDeclaration.identifier;
    const char *declared_type = node->varDeclaration.dataType;

    /* NUEVO (arrays): si el tipo es un pack, usa el chequeo de arrays */
    if (is_pack_type(declared_type)) {
        check_pack_declaration(node, kind);
        return;
    }

    const char *inferred = infer_type(node->varDeclaration.init);

    if (!type_matches(declared_type, inferred)) {
        fprintf(stderr,
                "[Semantic Error] %s '%s': declared type '%s' does not match value type '%s'\n",
                kind, name, declared_type, inferred);
        errors++;
    }
}

int semantic_analyze(ASTNode *root) {
    errors = 0;

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