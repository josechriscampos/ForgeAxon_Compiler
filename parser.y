%{
#include "ast.h"
#include "symbol_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>    ← AGREGAR ESTA LÍNEA

extern int yylex();
void yyerror(const char *s);

ASTNode *ast_root = NULL;
%}

%union {
    int int_val;
    double float_val;
    char *str_val;
    struct ASTNode *node;
    struct FunctionParameter *parameter;
}

%token TOKEN_MOVE
%token TOKEN_STAY
%token TOKEN_BEHAVIOR
%token TOKEN_GIVE
%token TOKEN_VOID
%token <str_val> TOKEN_TYPE
%token <str_val> TOKEN_IDENTIFIER
%token <str_val> TOKEN_STRING_LITERAL
%token <int_val> TOKEN_INT_LITERAL
%token <float_val> TOKEN_FLOAT_LITERAL
%token <int_val> TOKEN_BOOL_LITERAL

%type <node> program statement_list statement var_declaration const_declaration expression
%type <node> function_declaration function_call_statement
%type <node> function_statement_list function_statement give_statement
%type <node> argument_list optional_arguments
%type <parameter> parameter parameter_list optional_parameters
%type <str_val> return_type

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
    | function_declaration {
        $$ = $1;
    }
    | function_call_statement {
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

function_declaration:
    TOKEN_BEHAVIOR TOKEN_IDENTIFIER '(' optional_parameters ')' ':' return_type '{' function_statement_list '}' {
        $$ = create_function_decl($7, $2, $4, $9);
        symtab_insert($2, $7, SYM_FUNCTION);
        free($2);
        free($7);
    }
    ;

return_type:
    TOKEN_TYPE { $$ = $1; }
    | TOKEN_VOID { $$ = strdup("void"); }
    ;

optional_parameters:
    /* empty */ { $$ = NULL; }
    | parameter_list { $$ = $1; }
    ;

parameter_list:
    parameter { $$ = $1; }
    | parameter ',' parameter_list {
        $1->next = $3;
        $$ = $1;
    }
    ;

parameter:
    TOKEN_IDENTIFIER ':' TOKEN_TYPE {
        $$ = create_function_parameter($3, $1);
        free($1);
        free($3);
    }
    ;

function_statement_list:
    /* empty */ { $$ = NULL; }
    | function_statement function_statement_list {
        $1->next = $2;
        $$ = $1;
    }
    ;

function_statement:
    var_declaration ';' { $$ = $1; }
    | const_declaration ';' { $$ = $1; }
    | give_statement { $$ = $1; }
    ;

give_statement:
    TOKEN_GIVE expression ';' {
        $$ = create_give_node($2);
    }
    | TOKEN_GIVE ';' {
        $$ = create_give_node(NULL);
    }
    ;

function_call_statement:
    TOKEN_IDENTIFIER '(' optional_arguments ')' ';' {
        $$ = create_function_call($1, $3);
        free($1);
    }
    ;

optional_arguments:
    /* empty */ { $$ = NULL; }
    | argument_list { $$ = $1; }
    ;

argument_list:
    expression { $$ = $1; }
    | expression ',' argument_list {
        $1->next = $3;
        $$ = $1;
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