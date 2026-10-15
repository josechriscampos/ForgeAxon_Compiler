#include "symbol_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Symbol *head = NULL;

void symtab_init(void) {
    head = NULL;
}

int symtab_insert(const char *name, const char *dataType, SymbolCategory category) {
    if (symtab_lookup(name) != NULL) {
        return 0;
    }
    Symbol *s = (Symbol *)malloc(sizeof(Symbol));
    s->name = strdup(name);
    s->dataType = strdup(dataType);
    s->category = category;
    s->next = head;
    head = s;
    return 1;
}

Symbol* symtab_lookup(const char *name) {
    Symbol *cur = head;
    while (cur != NULL) {
        if (strcmp(cur->name, name) == 0) return cur;
        cur = cur->next;
    }
    return NULL;
}

static const char *category_name(SymbolCategory c) {
    switch (c) {
        case SYM_VARIABLE: return "variable";
        case SYM_CONSTANT: return "constant";
        case SYM_FUNCTION: return "behavior";
    }
    return "unknown";
}

void symtab_print(void) {
    printf("\n--- Symbol Table ---\n");
    printf("%-15s %-15s %-10s\n", "Name", "Type", "Category");
    printf("---------------------------------------------\n");
    Symbol *cur = head;
    while (cur != NULL) {
        printf("%-15s %-15s %-10s\n",
               cur->name,
               cur->dataType,
               category_name(cur->category));
        cur = cur->next;
    }
}

void symtab_free(void) {
    Symbol *cur = head;
    while (cur != NULL) {
        Symbol *next = cur->next;
        free(cur->name);
        free(cur->dataType);
        free(cur);
        cur = next;
    }
    head = NULL;
}