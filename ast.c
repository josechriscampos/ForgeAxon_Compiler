#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ASTNode *allocate_new_node() {
  ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
  if (node == NULL) {
    fprintf(stderr, "Out of memory\n");
    exit(EXIT_FAILURE);
  }
  return node;
}

ASTNode *create_int_node(int val) {
  ASTNode *node = allocate_new_node();
  node->type = INT_LITERAL;
  node->intLit.intValue = val;
  node->next = NULL;
  return node;
}

ASTNode *create_float_node(double val) {
  ASTNode *node = allocate_new_node();
  node->type = FLOAT_LITERAL;
  node->floatLit.floatValue = val;
  node->next = NULL;
  return node;
}

ASTNode *create_str_node(const char *val) {
  ASTNode *node = allocate_new_node();
  node->type = STR_LITERAL;
  node->strLit.strValue = strdup(val);
  node->next = NULL;
  return node;
}

ASTNode *create_bool_node(int val) {
  ASTNode *node = allocate_new_node();
  node->type = BOOL_LITERAL;
  node->boolLit.boolValue = val;
  node->next = NULL;
  return node;
}

ASTNode *create_var_decl(char *dtype, const char *name, ASTNode *init) {
  ASTNode *node = allocate_new_node();
  node->type = VAR_DECL;
  node->varDeclaration.dataType = strdup(dtype);
  node->varDeclaration.identifier = strdup(name);
  node->varDeclaration.init = init;
  node->next = NULL;
  return node;
}

ASTNode *create_const_decl(char *dtype, const char *name, ASTNode *init) {
  ASTNode *node = allocate_new_node();
  node->type = CONST_DECL;
  node->varDeclaration.dataType = strdup(dtype);
  node->varDeclaration.identifier = strdup(name);
  node->varDeclaration.init = init;
  node->next = NULL;
  return node;
}

static void print_init(ASTNode *init) {
  if (init->type == INT_LITERAL) {
    printf("  Init: IntegerLiteral(%d)\n", init->intLit.intValue);
  } else if (init->type == FLOAT_LITERAL) {
    printf("  Init: FloatLiteral(%g)\n", init->floatLit.floatValue);
  } else if (init->type == STR_LITERAL) {
    printf("  Init: StringLiteral(\"%s\")\n", init->strLit.strValue);
  } else if (init->type == BOOL_LITERAL) {
    printf("  Init: BooleanLiteral(%s)\n",
           init->boolLit.boolValue ? "true" : "false");
  }
}

void print_ast(ASTNode *node) {
  while (node != NULL) {
    if (node->type == VAR_DECL) {
      printf("VariableDeclaration:\n");
      printf("  Type: %s\n", node->varDeclaration.dataType);
      printf("  Identifier: %s\n", node->varDeclaration.identifier);
      if (node->varDeclaration.init) {
        print_init(node->varDeclaration.init);
      }
    } else if (node->type == CONST_DECL) {
      printf("ConstantDeclaration:\n");
      printf("  Type: %s\n", node->varDeclaration.dataType);
      printf("  Identifier: %s\n", node->varDeclaration.identifier);
      if (node->varDeclaration.init) {
        print_init(node->varDeclaration.init);
      }
    }
    node = node->next;
    if (node)
      printf("\n");
  }
}

void free_ast(ASTNode *node) {
  while (node != NULL) {
    ASTNode *next_node = node->next;

    switch (node->type) {
    case VAR_DECL:
    case CONST_DECL:
      if (node->varDeclaration.dataType) {
        free(node->varDeclaration.dataType);
      }
      if (node->varDeclaration.identifier) {
        free(node->varDeclaration.identifier);
      }
      if (node->varDeclaration.init) {
        free_ast(node->varDeclaration.init);
      }
      break;

    case STR_LITERAL:
      if (node->strLit.strValue) {
        free(node->strLit.strValue);
      }
      break;

    case INT_LITERAL:
    case FLOAT_LITERAL:
    case BOOL_LITERAL:
      break;
    }

    free(node);
    node = next_node;
  }
}