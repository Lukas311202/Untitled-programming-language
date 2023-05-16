#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct parameter_list parameter_list;
typedef struct node node;
typedef char* String;

typedef struct ast ast;
typedef struct exp_list exp_list;

typedef void (*func_ptr)(parameter_list* list);
typedef int (*int_func_ptr)(parameter_list* list);
typedef String (*string_func_ptr)(parameter_list* list);

typedef struct Variable Variable;

typedef struct function_container function_container;

typedef enum {
    interger_exp,
    string_exp,
    unary_exp,
    binary_exp,
    variable_exp,
    assign_exp,
    compare_exp,
    if_expression,
    callExp,
    declaration_exp,
    built_in_func_call
}tags;

struct Variable{
    tags tag;
    union {
        int interger_expr;
        String string_expr;
        void* expression;
    }value;
};

struct ast{
    tags tag;
    union {
        int intergerExp;
        String stringExp;
        String variableExp;
        struct {
            String operator;
            ast* left;
            ast* right;
        } binaryExp;

        struct {
            String name;
            exp_list* arguments;
        } callExp;
        struct {
            ast* variable;
            ast* value;
        } assignExp;
        struct {
            ast* left;
            ast* riht;
            String operator;
        }comp_exp;
        struct {
            ast* condition;
            ast* if_body;
            ast* else_body;
        }if_expression;
        struct {
            String name;
            exp_list* function;
        } declaration_exp;
        struct {
            String function_name;
            parameter_list* parameters;
        }built_in_func_call;
    }op;
};

struct exp_list{
    ast* elem;
    exp_list* next;
};

struct function_container{
    enum {
        VOID,
        INTEGER,
        STRING
    }return_value;
    
    union{
        func_ptr void_function;
        int_func_ptr int_function;
        string_func_ptr string_function;
    }function;
};

struct node{
    String var;
    void* value;
    Variable* val;
    tags tag;
    node* next;
};

struct parameter_list{
    node* first;
};

void* get_variable_value(parameter_list* list, String var);

void add_variable_value(parameter_list* list, String var, void* value);

void free_list(parameter_list* list);

parameter_list* make_list(node* n);

void print(parameter_list* list);
void it_worked(parameter_list* params);
int add(parameter_list* list);

typedef struct Token Token;

typedef enum {
    NUMBER,
    OPERATOR,
    OPERAND,
    ASSSIGNEMENT,
    NEW_LINE,
    PARAMETER,
    WORD,
    TAB,
    END
} TokenType;

struct Token{
    TokenType tag;
    union {
        char* function;
        int value;
        char* param;
        char operator;
    };
    char* expression;
    Token* next;
};

typedef struct TokenStack TokenStack;
struct TokenStack{
    Token* first;
};

TokenStack tokenize_expression(char* expression);
void print_Stack(TokenStack* t);
void free_tokens(Token* t);

void print_Token(Token* t, bool print_next);

exp_list* make_exp_list(ast* elem, exp_list* next);

ast* make_binary_expression(String operator, ast* left, ast* right);

ast* make_variableExp(String Variable);

ast* make_StringExp(String Variable);

ast* make_interger_exp(int value);

ast* make_assign_exp(ast* variable, ast* value);

ast* make_comp_exp(String operator, ast* left, ast* right);

ast* make_if_expression(ast* condition, ast* if_body, ast* else_body);

ast* make_function_call_exp(String name);

ast* make_declaration_exp(String name, exp_list* function);

ast* make_built_in_function_call(String name, parameter_list* parameters);

int execute(ast* e, parameter_list** params);
void execte_exp_list(exp_list* list, parameter_list** params);
void free_ext_list(exp_list* list);
void print_ast(ast* e);