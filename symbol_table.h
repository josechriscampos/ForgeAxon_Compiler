#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

typedef enum {
    SYM_VARIABLE,
    SYM_CONSTANT
} SymbolCategory;

typedef struct Symbol {
    char *name;
    char *dataType;
    SymbolCategory category;
    struct Symbol *next;
} Symbol;

/* API de la tabla de símbolos */
void symtab_init(void);
int  symtab_insert(const char *name, const char *dataType, SymbolCategory category);
Symbol* symtab_lookup(const char *name);
void symtab_print(void);
void symtab_free(void);

#endif