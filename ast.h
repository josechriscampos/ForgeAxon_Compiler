#ifndef AST_H
#define AST_H

typedef enum {
  VAR_DECL,
  CONST_DECL,
  INT_LITERAL,
  FLOAT_LITERAL,
  STR_LITERAL,
  BOOL_LITERAL,
} NodeType;

typedef struct VarDeclaration {
  char *dataType;
  char *identifier;
  struct ASTNode *init;
} VarDeclaration;

typedef struct ASTNode {
  NodeType type;
  union {
    struct VarDeclaration varDeclaration;
    struct {
      int intValue;
    } intLit;
    struct {
      double floatValue;
    } floatLit;
    struct {
      char *strValue;
    } strLit;
    struct {
      int boolValue;
    } boolLit;
  };

  struct ASTNode *next;

} ASTNode;


ASTNode* create_int_node(int val);
ASTNode* create_float_node(double val);
ASTNode* create_str_node(const char *val);
ASTNode* create_bool_node(int val);
ASTNode* create_var_decl(char *dtype, const char *name, ASTNode *init);
ASTNode* create_const_decl(char *dtype, const char *name, ASTNode *init);
void print_ast(ASTNode *root);
void free_ast(ASTNode *node);

#endif