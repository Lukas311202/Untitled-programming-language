#include "Abstract_Syntax_tree.h"

void execte_exp_list();
void free_ext_list();

exp_list* make_exp_list(ast* elem, exp_list* next){
    exp_list* list = malloc(sizeof(exp_list));
    list->elem = elem;
    list->next = next;
    return list;
}

ast* make_binary_expression(String operator, ast* left, ast* right){
    ast* e = (ast*) malloc(sizeof(ast));
    e->tag = binary_exp;
    e->op.binaryExp.left = left;
    e->op.binaryExp.right = right;
    e->op.binaryExp.operator = operator;
    return e;
}

ast* make_variableExp(String Variable){
    ast* e = malloc(sizeof(ast));
    e->tag = variable_exp;
    e->op.variableExp = Variable;
    return e;
}

ast* make_StringExp(String Variable){
    ast* e = malloc(sizeof(ast));
    e->tag = string_exp;
    e->op.stringExp = Variable;
    return e;
}

ast* make_interger_exp(int value){
    ast* e = malloc(sizeof(ast));
    e->tag = interger_exp;
    e->op.intergerExp = value;
    return e;
}

ast* make_assign_exp(ast* variable, ast* value){
    ast* e = malloc(sizeof(ast));
    e->tag = assign_exp;
    e->op.assignExp.variable = variable;
    e->op.assignExp.value = value;
    return e;
}

ast* make_comp_exp(String operator, ast* left, ast* right){
    ast* e = malloc(sizeof(ast));
    e->tag = compare_exp;
    e->op.comp_exp.operator = operator;
    e->op.comp_exp.left = left;
    e->op.comp_exp.riht = right;
    return e;
}

ast* make_if_expression(ast* condition, ast* if_body, ast* else_body){
    ast* e = malloc(sizeof(ast));
    e->tag = if_expression;
    e->op.if_expression.condition = condition;
    e->op.if_expression.if_body = if_body;
    e->op.if_expression.else_body = else_body;
    return e;
}

ast* make_function_call_exp(String name){
    ast* e = malloc(sizeof(ast));
    e->tag = callExp;
    e->op.callExp.name = name;
    return e;
}

ast* make_declaration_exp(String name, exp_list* function){
    ast* e = malloc(sizeof(ast));
    e->tag = declaration_exp; 
    e->op.declaration_exp.name = name;
    e->op.declaration_exp.function = function;
    return e;
}

ast* make_built_in_function_call(String name, parameter_list* parameters){
    ast* e = malloc(sizeof(ast));
    e->tag = built_in_func_call;
    e->op.built_in_func_call.function_name = name;
    e->op.built_in_func_call.parameters = parameters;
    return e;
}

void print_ast(ast* e){
    if(e == NULL) return;
    // printf("print ast\n");
    switch (e->tag)
    {
    case interger_exp:
        printf("%d", e->op.intergerExp);
        break;
    case binary_exp:
        print_ast(e->op.binaryExp.left);
        printf(" %s ", e->op.binaryExp.operator);
        print_ast(e->op.binaryExp.right);
        break;
    case variable_exp:
        printf("%s", e->op.variableExp);
        break;
    case string_exp:
        printf("%s", e->op.stringExp);
        break;
    case assign_exp:
        printf("%s = ", e->op.assignExp.variable->op.variableExp);
        print_ast(e->op.assignExp.value);
        break;
    case compare_exp:
        print_ast(e->op.comp_exp.left);
        printf(" %s ", e->op.comp_exp.operator);
        print_ast(e->op.comp_exp.riht);
        break;
    case if_expression:
        printf("if ");
        print_ast(e->op.if_expression.condition);
        printf(":\n\t");
        print_ast(e->op.if_expression.if_body);
        if(e->op.if_expression.else_body == NULL) return;
        printf("\nelse:\n\t");
        print_ast(e->op.if_expression.else_body);
        break;
    case callExp:
        printf("%s()", e->op.callExp.name);
        break;
    case declaration_exp:
        printf("fn %s():", e->op.declaration_exp.name);
        for(exp_list* elem = e->op.declaration_exp.function; elem != NULL; elem = elem->next){
            printf("\n\t");
            print_ast(elem->elem);
        }
        printf("\n");
        break;
    case built_in_func_call:
        printf("%s()", e->op.built_in_func_call.function_name);
    default:
        printf("unknown token");
        break;
    }
}

void free_ast(ast* e){
    if(e == NULL) return;
    switch (e->tag)
    {
    case interger_exp:
        free(e);
        break;
    case string_exp:
        free(e);
        break;
    case variable_exp:
        free(e);
        break;
    case binary_exp:
        free_ast(e->op.binaryExp.left);
        free_ast(e->op.binaryExp.right);
        free(e);
        break;
    case assign_exp:
        free_ast(e->op.assignExp.value);
        free(e);
        break;
    case compare_exp:
        free_ast(e->op.comp_exp.left);
        free_ast(e->op.comp_exp.riht);
        free(e);
        break;
    case if_expression:
        printf("free if expression\n");
        free_ast(e->op.if_expression.condition);
        free_ast(e->op.if_expression.if_body);
        if(e->op.if_expression.else_body != NULL) free_ast(e->op.if_expression.else_body);
        free(e);
        break;
    case declaration_exp:
        free_ext_list(e->op.declaration_exp.function);
        free(e);
        break;
    case callExp:
        free(e);
        break;
    case built_in_func_call:
        free(e);
        break;
    default:
        printf("no free function for token %d found\n", e->tag);
        break;
    }
    // printf("\nast freed\n");
}

void free_ext_list(exp_list* list){
    if(list == NULL) return;
    free_ext_list(list->next);
    free_ast(list->elem);
    free(list);
}

int execute();

int execute_binary_expression(ast* e, parameter_list** params){
    int left = execute(e->op.binaryExp.left, params);
    int right = execute(e->op.binaryExp.right, params);
    if (strcmp(e->op.binaryExp.operator, "-") == 0)
        return left - right;
    if (strcmp(e->op.binaryExp.operator, "+") == 0)
        return left + right;
    if (strcmp(e->op.binaryExp.operator, "*") == 0)
        return left * right;
    if (strcmp(e->op.binaryExp.operator, "/") == 0)
        return left / right;
}

void print_parameter_list(parameter_list* list){
    printf("print the list\n");
    for(node* n = list->first; n != NULL; n = n->next){
        printf("%s has the value ", n->var);
        print_ast((ast*)n->value);
        printf("\n");
    }
}

int execute_compare_expression(ast* e, parameter_list** params){
    int left = execute(e->op.comp_exp.left, params);
    int right = execute(e->op.comp_exp.riht, params);
    if(strcmp(e->op.comp_exp.operator, ">") == 0) return left > right;
    if(strcmp(e->op.comp_exp.operator, ">=") == 0) return left >= right;
    if(strcmp(e->op.comp_exp.operator, "<") == 0) return left < right;
    if(strcmp(e->op.comp_exp.operator, "<=") == 0) return left <= right;
    if(strcmp(e->op.comp_exp.operator, "==") == 0) return left == right;
    if(strcmp(e->op.comp_exp.operator, "!=") == 0) return left != right;
}

int execute(ast* e, parameter_list** params){
    switch (e->tag)
    {
    case interger_exp:
        return e->op.intergerExp;
    case string_exp:
        printf("%s\n", e->op.stringExp);
        return 1;
    case binary_exp:
        return execute_binary_expression(e, params);
    case variable_exp:
        // int v = execute((ast*)get_variable_value(*params, e->op.variableExp), params);
        printf("value is %d\n", execute((ast*)get_variable_value(*params, e->op.variableExp), params));
        //todo change that so that it returns the value and not a refernce to the ast holding it
        return execute((ast*)get_variable_value(*params, e->op.variableExp), params);
    case assign_exp:
        add_variable_value(*params, e->op.assignExp.variable->op.variableExp,  e->op.assignExp.value);
        return 1;
    case compare_exp:
        return execute_compare_expression(e, params);
    case if_expression: 
        if(execute(e->op.if_expression.condition, params) == 1){
            printf("condition was true\n");
            return execute(e->op.if_expression.if_body, params);
        }else if(e->op.if_expression.else_body != NULL) {
            printf("condition was false\n");
            return execute(e->op.if_expression.else_body, params);}
    case callExp:
        printf("call function %s\n", e->op.callExp.name);
        // exp_list* list = (exp_list*)get_variable_value(*params, e->op.callExp.name);
        execte_exp_list((exp_list*)get_variable_value(*params, e->op.callExp.name), params);
        return 1;
    case declaration_exp:
        printf("add function %s to the parameter list\n", e->op.declaration_exp.name);
        add_variable_value(*params, e->op.declaration_exp.name, e->op.declaration_exp.function);
        return 1;
    case built_in_func_call:
        printf("call built in function %s\n", e->op.built_in_func_call.function_name);
        function_container* fn = get_variable_value(*params, e->op.built_in_func_call.function_name);
        switch(fn->return_value){
            case VOID:
                fn->function.void_function(e->op.built_in_func_call.parameters);
                return 1;
            case INTEGER:
                return fn->function.int_function(e->op.built_in_func_call.parameters);
        }
    }
}

void execte_exp_list(exp_list* list, parameter_list** params){
    for(exp_list* l = list; l != NULL; l = l->next){
        // print_ast(l->elem);
        // printf("\n");
        printf("result is %d\n", execute(l->elem, params));
        // print_parameter_list(params);
    }
}

int main(int argc, char** argv){
    ast* ast1 = make_assign_exp(make_variableExp("s"), make_interger_exp(20));
    ast* ast2 = make_binary_expression("*", make_variableExp("s"), make_interger_exp(5));
    exp_list* list = make_exp_list(ast1, make_exp_list(ast2, NULL));
    parameter_list* params = make_list(NULL);
    execte_exp_list(list, &params);
    free_ext_list(list);
    free_list(params);
    return 0;
}