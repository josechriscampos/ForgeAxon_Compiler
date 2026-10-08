%{
#include "ast.h"
#include "symbol_table.h"
#include <stdio.h>
#include <stdlib.h>

extern int yylex();
void yyerror(const char *s);

ASTNode *ast_root = NULL;
%}

%union {
    int int_val;
    double float_val;
    char *str_val;
    struct ASTNode *node;
}

%token TOKEN_MOVE
%token TOKEN_STAY
%token <str_val> TOKEN_TYPE
%token <str_val> TOKEN_IDENTIFIER
%token <str_val> TOKEN_STRING_LITERAL
%token <int_val> TOKEN_INT_LITERAL
%token <float_val> TOKEN_FLOAT_LITERAL
%token <int_val> TOKEN_BOOL_LITERAL

%type <node> program statement_list statement var_declaration const_declaration expression

%%

program:
    statement_list {
        ast_root = $1;
    }
    ;

statement_list:
    statement {
        $$ = $1;
    }
    | statement_list statement {
        ASTNode *cur = $1;
        while (cur->next != NULL) {
            cur = cur->next;
        }
        cur->next = $2;
        $$ = $1;
    }
    ;

statement:
    var_declaration ';' {
        $$ = $1;
    }
    | const_declaration ';' {
        $$ = $1;
    }
    ;

var_declaration:
    TOKEN_MOVE TOKEN_IDENTIFIER '=' expression {
        $$ = create_var_decl("inferred", $2, $4);
        symtab_insert($2, "inferred", SYM_VARIABLE);
        free($2);
    }
    | TOKEN_MOVE TOKEN_IDENTIFIER ':' TOKEN_TYPE '=' expression {
        $$ = create_var_decl($4, $2, $6);
        symtab_insert($2, $4, SYM_VARIABLE);
        free($2);
        free($4);
    }
    ;

const_declaration:
    TOKEN_STAY TOKEN_IDENTIFIER '=' expression {
        $$ = create_const_decl("inferred", $2, $4);
        symtab_insert($2, "inferred", SYM_CONSTANT);
        free($2);
    }
    | TOKEN_STAY TOKEN_IDENTIFIER ':' TOKEN_TYPE '=' expression {
        $$ = create_const_decl($4, $2, $6);
        symtab_insert($2, $4, SYM_CONSTANT);
        free($2);
        free($4);
    }
    ;

expression:
    TOKEN_INT_LITERAL {
        $$ = create_int_node($1);
    }
    | TOKEN_FLOAT_LITERAL {
        $$ = create_float_node($1);
    }
    | TOKEN_STRING_LITERAL {
        $$ = create_str_node($1);
        free($1);
    }
    | TOKEN_BOOL_LITERAL {
        $$ = create_bool_node($1);
    }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Parse error: %s\n", s);
}