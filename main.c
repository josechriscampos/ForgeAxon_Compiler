#include "ast.h"
#include "symbol_table.h"
#include "semantic.h"
#include <stdio.h>
#include <stdlib.h>

extern FILE *yyin;
extern int yyparse(void);
extern ASTNode *ast_root;

static int read_input(int argc, char **argv) {
  if (argc > 1) {
    FILE *file = fopen(argv[1], "r");
    if (!file) {
      perror("Error opening the file");
      return -1;
    }
    yyin = file;
  } else {
    yyin = stdin;
    printf("Reading from standard input (Ctrl+D to end)...\n");
  }
  return 0;
}

static void cleanup_lexer_input(void) {
  if (yyin && yyin != stdin) {
    fclose(yyin);
    yyin = NULL;
  }
}

int main(int argc, char **argv) {
  if (read_input(argc, argv) != 0) {
    return EXIT_FAILURE;
  }

  symtab_init();

  int parse_result = yyparse();

  cleanup_lexer_input();

  if (parse_result == 0) {
    printf("--- AST Successfully Built ---\n");
    print_ast(ast_root);

    symtab_print();

    printf("\n--- Semantic Analysis ---\n");
    int sem_errors = semantic_analyze(ast_root);
    if (sem_errors == 0) {
      printf("No semantic errors found.\n");
    } else {
      printf("%d semantic error(s) found.\n", sem_errors);
    }

    free_ast(ast_root);
    symtab_free();
    return sem_errors == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
  } else {
    fprintf(stderr, "Parsing failed due to syntax errors.\n");
    symtab_free();
    return EXIT_FAILURE;
  }
}