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

ASTNode *create_identifier_ref(const char *name) {
  ASTNode *node = allocate_new_node();
  node->type = IDENTIFIER_REF;
  node->identifierRef.name = strdup(name);
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

FunctionParameter *create_function_parameter(const char *dtype, const char *name) {
  FunctionParameter *p = (FunctionParameter *)malloc(sizeof(FunctionParameter));
  if (p == NULL) {
    fprintf(stderr, "Out of memory\n");
    exit(EXIT_FAILURE);
  }
  p->dataType = strdup(dtype);
  p->identifier = strdup(name);
  p->next = NULL;
  return p;
}

ASTNode *create_function_decl(const char *returnType, const char *name,
                              FunctionParameter *parameters, ASTNode *body) {
  ASTNode *node = allocate_new_node();
  node->type = FUNCTION_DECL;
  node->functionDeclaration.returnType = strdup(returnType);
  node->functionDeclaration.identifier = strdup(name);
  node->functionDeclaration.parameters = parameters;
  node->functionDeclaration.body = body;
  node->next = NULL;
  return node;
}

ASTNode *create_function_call(const char *name, ASTNode *arguments) {
  ASTNode *node = allocate_new_node();
  node->type = FUNCTION_CALL;
  node->functionCall.identifier = strdup(name);
  node->functionCall.arguments = arguments;
  node->next = NULL;
  return node;
}

ASTNode *create_give_node(ASTNode *value) {
  ASTNode *node = allocate_new_node();
  node->type = GIVE_STMT;
  node->giveStatement.value = value;
  node->next = NULL;
  return node;
}

ASTNode *create_if_node(ASTNode *condition, ASTNode *thenBody, ASTNode *elseBody) {
  ASTNode *node = allocate_new_node();
  node->type = IF_STMT;
  node->ifStatement.condition = condition;
  node->ifStatement.thenBody = thenBody;
  node->ifStatement.elseBody = elseBody;
  node->next = NULL;
  return node;
}

static void print_indent(int depth) {
  for (int i = 0; i < depth; i++) printf("  ");
}

static void print_ast_node(ASTNode *node, int depth);

static void print_value(ASTNode *v, int depth) {
  print_indent(depth);
  if (v->type == INT_LITERAL) {
    printf("IntegerLiteral(%d)\n", v->intLit.intValue);
  } else if (v->type == FLOAT_LITERAL) {
    printf("FloatLiteral(%g)\n", v->floatLit.floatValue);
  } else if (v->type == STR_LITERAL) {
    printf("StringLiteral(\"%s\")\n", v->strLit.strValue);
  } else if (v->type == BOOL_LITERAL) {
    printf("BooleanLiteral(%s)\n", v->boolLit.boolValue ? "true" : "false");
  } else if (v->type == IDENTIFIER_REF) {
    printf("IdentifierRef(%s)\n", v->identifierRef.name);
  }
}

static void print_init(ASTNode *init, int depth) {
  print_indent(depth);
  printf("Init:\n");
  print_value(init, depth + 1);
}

static void print_ast_node(ASTNode *node, int depth) {
  print_indent(depth);
  switch (node->type) {
    case VAR_DECL:
      printf("VariableDeclaration: %s (Type: %s)\n",
             node->varDeclaration.identifier,
             node->varDeclaration.dataType);
      if (node->varDeclaration.init) print_init(node->varDeclaration.init, depth + 1);
      break;
    case CONST_DECL:
      printf("ConstantDeclaration: %s (Type: %s)\n",
             node->varDeclaration.identifier,
             node->varDeclaration.dataType);
      if (node->varDeclaration.init) print_init(node->varDeclaration.init, depth + 1);
      break;
    case FUNCTION_DECL: {
      FunctionDeclaration *fn = &node->functionDeclaration;
      printf("Behavior: %s -> %s\n", fn->identifier, fn->returnType);
      print_indent(depth + 1); printf("Parameters:\n");
      if (fn->parameters == NULL) {
        print_indent(depth + 2); printf("<none>\n");
      }
      for (FunctionParameter *p = fn->parameters; p != NULL; p = p->next) {
        print_indent(depth + 2);
        printf("%s: %s\n", p->identifier, p->dataType);
      }
      print_indent(depth + 1); printf("Body:\n");
      if (fn->body == NULL) {
        print_indent(depth + 2); printf("<empty>\n");
      } else {
        for (ASTNode *stmt = fn->body; stmt != NULL; stmt = stmt->next) {
          print_ast_node(stmt, depth + 2);
        }
      }
      break;
    }
    case FUNCTION_CALL:
      printf("Call: %s\n", node->functionCall.identifier);
      print_indent(depth + 1); printf("Arguments:\n");
      if (node->functionCall.arguments == NULL) {
        print_indent(depth + 2); printf("<none>\n");
      } else {
        for (ASTNode *arg = node->functionCall.arguments; arg != NULL; arg = arg->next) {
          print_value(arg, depth + 2);
        }
      }
      break;
    case GIVE_STMT:
      printf("Give\n");
      if (node->giveStatement.value) {
        print_value(node->giveStatement.value, depth + 1);
      } else {
        print_indent(depth + 1); printf("<void>\n");
      }
      break;
    case IF_STMT:
      printf("When\n");
      print_indent(depth + 1); printf("Condition:\n");
      print_value(node->ifStatement.condition, depth + 2);
      print_indent(depth + 1); printf("Then:\n");
      if (node->ifStatement.thenBody == NULL) {
        print_indent(depth + 2); printf("<empty>\n");
      } else {
        for (ASTNode *s = node->ifStatement.thenBody; s != NULL; s = s->next) {
          print_ast_node(s, depth + 2);
        }
      }
      if (node->ifStatement.elseBody != NULL) {
        print_indent(depth + 1); printf("Otherwise:\n");
        for (ASTNode *s = node->ifStatement.elseBody; s != NULL; s = s->next) {
          print_ast_node(s, depth + 2);
        }
      }
      break;
    case IDENTIFIER_REF:
      printf("IdentifierRef(%s)\n", node->identifierRef.name);
      break;
    default:
      break;
  }
}

void print_ast(ASTNode *node) {
  while (node != NULL) {
    print_ast_node(node, 0);
    node = node->next;
    if (node) printf("\n");
  }
}

void free_ast(ASTNode *node) {
  while (node != NULL) {
    ASTNode *next_node = node->next;

    switch (node->type) {
    case VAR_DECL:
    case CONST_DECL:
      if (node->varDeclaration.dataType) free(node->varDeclaration.dataType);
      if (node->varDeclaration.identifier) free(node->varDeclaration.identifier);
      if (node->varDeclaration.init) free_ast(node->varDeclaration.init);
      break;

    case FUNCTION_DECL: {
      FunctionDeclaration *fn = &node->functionDeclaration;
      free(fn->returnType);
      free(fn->identifier);
      FunctionParameter *p = fn->parameters;
      while (p != NULL) {
        FunctionParameter *next = p->next;
        free(p->dataType);
        free(p->identifier);
        free(p);
        p = next;
      }
      free_ast(fn->body);
      break;
    }

    case FUNCTION_CALL:
      free(node->functionCall.identifier);
      free_ast(node->functionCall.arguments);
      break;

    case GIVE_STMT:
      if (node->giveStatement.value) free_ast(node->giveStatement.value);
      break;

    case IF_STMT:
      if (node->ifStatement.condition) free_ast(node->ifStatement.condition);
      if (node->ifStatement.thenBody) free_ast(node->ifStatement.thenBody);
      if (node->ifStatement.elseBody) free_ast(node->ifStatement.elseBody);
      break;

    case IDENTIFIER_REF:
      if (node->identifierRef.name) free(node->identifierRef.name);
      break;

    case STR_LITERAL:
      if (node->strLit.strValue) free(node->strLit.strValue);
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