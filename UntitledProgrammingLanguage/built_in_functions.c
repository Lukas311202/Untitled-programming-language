#include "Abstract_Syntax_tree.h"

void print(parameter_list* parameters){
    printf("\nprint something:\n");
    for(node* n = parameters->first; n != NULL; n = n->next){
        printf("%s ", n->var);
    }
    printf("\n\n");
}

void it_worked(parameter_list* params){
    printf("it worked\n");
}

int add(parameter_list* parameters){
    int sum = 0;
    for(node* n = parameters->first; n != NULL; n = n->next){
        // printf("%s ", n->var);
        printf("add %d\n", *(int*)n->value);
        sum += *(int*)n->value;
    }
    return sum;
}