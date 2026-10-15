#ifndef AST_H
#define AST_H

typedef enum {
  VAR_DECL,
  CONST_DECL,
  INT_LITERAL,
  FLOAT_LITERAL,
  STR_LITERAL,
  BOOL_LITERAL,
  FUNCTION_DECL,
  FUNCTION_CALL,
  GIVE_STMT,
} NodeType;

typedef struct VarDeclaration {
  char *dataType;
  char *identifier;
  struct ASTNode *init;
} VarDeclaration;

typedef struct FunctionParameter {
  char *dataType;
  char *identifier;
  struct FunctionParameter *next;
} FunctionParameter;

typedef struct FunctionDeclaration {
  char *returnType;
  char *identifier;
  FunctionParameter *parameters;
  struct ASTNode *body;
} FunctionDeclaration;

typedef struct FunctionCall {
  char *identifier;
  struct ASTNode *arguments;
} FunctionCall;

typedef struct ASTNode {
  NodeType type;
  union {
    struct VarDeclaration varDeclaration;
    struct FunctionDeclaration functionDeclaration;
    struct FunctionCall functionCall;
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
    struct {
      struct ASTNode *value;
    } giveStatement;
  };

  struct ASTNode *next;

} ASTNode;


ASTNode* create_int_node(int val);
ASTNode* create_float_node(double val);
ASTNode* create_str_node(const char *val);
ASTNode* create_bool_node(int val);
ASTNode* create_var_decl(char *dtype, const char *name, ASTNode *init);
ASTNode* create_const_decl(char *dtype, const char *name, ASTNode *init);
FunctionParameter* create_function_parameter(const char *dtype, const char *name);
ASTNode* create_function_decl(const char *returnType, const char *name,
                              FunctionParameter *parameters, ASTNode *body);
ASTNode* create_function_call(const char *name, ASTNode *arguments);
ASTNode* create_give_node(ASTNode *value);
void print_ast(ASTNode *root);
void free_ast(ASTNode *node);

#endif