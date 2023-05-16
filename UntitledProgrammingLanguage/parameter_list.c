#include "Abstract_Syntax_tree.h"


Variable* make_integer_variable(int value){
    Variable* v = malloc(sizeof(Variable));
    v->tag = interger_exp;
    v->value.interger_expr = value;
    return v;
}

void free_variable(Variable* v){
    free(v);
}

node* make_node(String var, void* value){
    node* n = malloc(sizeof(node));
    n->var = var;
    n->value = value;
    return n;
}

void* get_variable_value(parameter_list* list, String var){
    for(node* n = list->first; n != NULL; n = n->next){
        if(strcmp(n->var, var) == 0){
            return n->value;
        }
    }
    return NULL;
}

void change_value(parameter_list* list, String var, void* new_value){
    for(node* n = list->first; n != NULL; n = n->next){
        if(strcmp(n->var, var) == 0){
            n->value = new_value;
            printf("value changed\n");
            break;
        }
    }
}

void add_variable_value(parameter_list* list, String var, void* value){
    if(get_variable_value(list, var) != NULL){
        printf("%s already exists and will be changed\n", var);
        change_value(list, var, value);
        return; 
    }

    if(list->first == NULL){
        list->first = make_node(var, value);
        printf("%s was added at the first place\n", var);
        return;
    }
    node* n = list->first;
    while(n->next != NULL) {n = n->next;}
    n->next = make_node(var, value);
    printf("%s was added to the list\n", var);
}

void free_node(node* n){
    if(n == NULL) return;
    free_node(n->next);
    printf("%s freed\n", n->var);
    free(n);
}

void free_list(parameter_list* list){
    free_node(list->first);
    free(list);
    printf("list freed\n");
}

parameter_list* make_list(node* n){
    parameter_list* list = malloc(sizeof(parameter_list));
    list->first = n;
    return list;
}