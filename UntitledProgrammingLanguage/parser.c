#include "Abstract_Syntax_tree.h"


// ast* parse_t(Token* t){
//     switch (t->tag)
//     {
//     case NUMBER:
//         printf("return %d\n", t->value);
//         return make_interger_exp(t->value);
    
//     default:
//         return NULL;
//     }
// }

// ast* parse_e(Token* t){
//     ast* a = parse_t(t);
//     Token* next_token = t->next;
//     while (true)
//     {
//         if(next_token->tag == OPERATOR){
//             char* op = &next_token->operator; 
//             next_token = next_token->next;
//             ast* b = parse_t(next_token);
//             a = make_binary_expression(op, a, b);
//         } else return a;
//     }
//     return a;
// }

int get_hierarchy(ast* a){
    switch (a->tag)
    {
    case interger_exp: return 100;
    case binary_exp: 
        if(strcmp(a->op.binaryExp.operator, "*") == 0 | strcmp(a->op.binaryExp.operator, "/") == 0) return 90;
        return 80;
    case assign_exp: return 70;
    case variable_exp: return 100;
    
    default: return 0;
    }
}

void attach_to_tree(ast** tree, ast* a){
    if(*tree == NULL) {
        printf("first place\n");
        *tree = a;
        return;
    }
    
    ast* root = *tree;
    int root_hierarchy = get_hierarchy(root);
    int a_hierarchy = get_hierarchy(a);
    
    switch (a->tag)
    {
        case binary_exp: ;

            if(root_hierarchy >= a_hierarchy){
                a->op.binaryExp.left = root;
                *tree = a;
            }else{
                //the new node is the child

                if(root->tag == binary_exp) attach_to_tree(&root->op.binaryExp.right, a);
                else if (root->tag == assign_exp) attach_to_tree(&root->op.assignExp.value, a);
            }

            return;
        case interger_exp:
            if(root->tag == binary_exp) attach_to_tree(&root->op.binaryExp.right, a);
            else if (root->tag == assign_exp) attach_to_tree(&root->op.assignExp.value, a);
            return;
        case assign_exp:
            a->op.assignExp.variable = root;
            *tree = a;
            return;
        case variable_exp: ;
            if(root->tag == assign_exp) {
                printf("variable goes under assignment\n");
                attach_to_tree(&root->op.assignExp.value, a);
                }
            return;
        
        default:
            break;
    }
}

exp_list* parse_token(Token* tok){
    exp_list* list = make_exp_list(NULL, NULL);
    exp_list** current = &list;
    ast* a = NULL;
    for(Token* t = tok; t->tag != END; t = t->next){
        printf("attach ");
        print_Token(t, false);
        printf("\n");
        switch (t->tag)
        {
            case NUMBER:
                attach_to_tree(&a, make_interger_exp(t->value));
                break;
            case OPERATOR: ;
                attach_to_tree(&a, make_binary_expression(&t->operator, NULL, NULL));
                break;
            case ASSSIGNEMENT: ;
                attach_to_tree(&a, make_assign_exp(NULL, NULL));
                // ast* prev = a;
                // char* op = &t->operator;
                // ast* next = parse_token(t->next);
                // a = make_binary_expression(op, prev, next);
                // return a;
                break;
            case WORD: ;
                printf("%s\n", t->expression);
                attach_to_tree(&a, make_variableExp(t->expression));
                break;
            case NEW_LINE: ;
                printf("start new expression\n");
                (*current)->elem = a;
                (*current)->next = make_exp_list(NULL, NULL);
                current = &(*current)->next;
                a = NULL;
            
            default:
                break;
        }
        printf("updated tree\n");
        print_ast(a);
        printf("\n");
    }
    (*current)->elem = a;
    return list;
}

exp_list* parse_to_exp_list(Token* tok){
    exp_list* list = parse_token(tok);
}

void parse(TokenStack* t){
    printf("\nPARSE TOKEN\n");
    // ast* prog = parse_token(t->first);
    // ast* prog = make_assign_exp(make_variableExp("x"), make_binary_expression("+", make_interger_exp(20), make_interger_exp(6)));
    exp_list* list = parse_to_exp_list(t->first);
    parameter_list* params = make_list(NULL);
    printf("expression parsed\n ");
    printf("\n");
    execte_exp_list(list, &params);
    int x_value = execute((ast*)get_variable_value(params, "x"), &params);
    printf("x is %d\n", x_value);
    free_ext_list(list);
    free_list(params);
}

int main(int argc, char* argv[]){

    char buffer[100];
    char c = getchar();
    int idx = 0;
    while (c != EOF)
    {
        // printf("%c", c);
        //todo replace 4 spaces with tab
        buffer[idx++] = c;
        c = getchar();
    }
    buffer[idx] = 0;
    TokenStack t = tokenize_expression(buffer);
    print_Stack(&t);
    parse(&t);
    free_tokens(t.first);
    return 0;
}