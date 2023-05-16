#include "Abstract_Syntax_tree.h"

char* TokenTypeArr[] = {"Number", "Operator", "Operand", "Assignment", "New line", "Parameter", "id", "Tab", "END"};

Token* make_operator_token(char op){
    Token* t = malloc(sizeof(Token));
    t->tag = OPERATOR;
    t->operator = op;
    return t;
}

Token* make_operand_token(int n){
    Token* t = malloc(sizeof(Token));
    t->tag = NUMBER;
    t->value = n;
    return t;
}

Token* make_end_token(){
    Token* t = malloc(sizeof(Token));
    t->tag = END;
    return t;
}

Token* make_token(char* expressiom, TokenType tag, Token* next){
    printf("make token\n");
    Token* t = malloc(sizeof(Token));
    t->tag = tag;
    // strcpy(t->expression, expressiom);
    t->expression = expressiom;
    t->next = next;
    return t;
}

bool end_of_token(char c){
    return c == ' ' || c == 0;
}

bool is_whitespace(char c){
    return c == ' ';
}

void add_token(TokenStack* stack, Token* new_token){
    if(stack->first == NULL) {
//         printf("put in first place\n");
        stack->first = new_token;
        return;
    }
    Token* list = stack->first;
    for(; list->next != NULL; list = list->next);
    list->next = new_token;
//     printf("added token\n");
}

//skips whitespaces and returns the new position
int skip_whitespace(char* expression, int idx){
    while(expression[idx] == ' ') idx++;
    return idx;
}

int jump_end_of_token(char* expression, int idx){
    while(expression[idx] != ' ' && expression[idx] != 0) idx++;
    return idx;
}

bool is_operator(char expression){
    return (expression == '/') || (expression == '-') || (expression == '+') || (expression == '*');
}

//for now only intergers ie no floating point
bool is_operand(char* expression){
    for(int idx = 0; expression[idx] != 0; idx++){
        if(!('0' <= expression[idx] && expression[idx]  <= '9')) return false;
    }
    return true;
}

bool is_assignment(char* expression){
    if(strcmp(expression, "=") == 0) return true;
    return false;
}

char* copy_substring(char* text, int start, int end){
    if(start == end){
        char* s = malloc(2 * sizeof(char));
        s[0] = text[start];
        s[1] = 0;
        return s;
    }
    char* s = malloc(sizeof(char) * (end - start) + 1);
    int i = start;
    for(; i< end; i++){
        s[i-start] = text[i];
    }
    s[end-start+1] = '\0';
    return s;
}

TokenType determine_type(char* expression){
    if (is_operator(expression[0])) return OPERATOR;
    if( is_assignment(expression)) return ASSSIGNEMENT;
    if (is_operand(expression)) return OPERAND;
    if(strcmp(expression, "\n") == 0) return NEW_LINE;
    return PARAMETER;
}

char* end_of_word(int* start, int* end, int idx, char* e){
    printf("\nend of word reached\n");
    char* word = copy_substring(e, *start, *end-1);
    printf("Word: %s\n", word);
    *start = -1;
    // *end = *start;
    return word;
}

void print_sub_string(int start, int end, char* e){
    printf("\n");
    for(int x = start; x < end; x++){
        printf("%c", e[x]);
    }

    int len = end - start;
    printf(": %d\n", len);
}

bool is_id_token(char c){
    return !(c == '\n' || c == '\t' || is_operator(c) || c == '=' || c == ' ');
}

TokenStack tokenize_expression(char* expression){
    printf("\nTOKENIZE EXPRESSION\n");
    TokenStack t;
    t.first = NULL;

    int start = 0;   
    char* last_word;
    int idx = 0;
    while(expression[idx] != 0 ){
        char c = expression[idx];

        if(is_id_token(c)){
            if(start == -1) start = idx;
        }else{
            if(start != -1){
                // print_sub_string(start, idx, expression);
                char* word = copy_substring(expression, start, idx);
                printf("%s\n", word);  
                if(is_operand(word)){
                    add_token(&t, make_operand_token(atoi(word)));
                }else {
                    Token* word_token = make_token(word, WORD, NULL);
                    printf("word = %s\n", word_token->expression);
                    add_token(&t, word_token);
                    }   
                start = -1;
            }

            if(c == '\n'){
                // printf("[NEW LINE]");
                add_token(&t, make_token("NEW LINE", NEW_LINE, NULL));
                }
            else if(c == '\t'){
                // printf("[TAB]");
                add_token(&t, make_token("TAB", TAB, NULL));
            }
            else if(c == ' ') {
                idx++;
                continue;
                }
            else if(c == '='){
                // printf("= => Assignment");
                add_token(&t, make_token("=", ASSSIGNEMENT, NULL));
                }
            else if(is_operator(c)) {
                // printf("%c => Operator", c);
                // char* operator = copy_substring(expression, idx, idx);
                add_token(&t, make_operator_token(c));
                }
            // printf("\n");
        }
        idx++;
        }
    
    if(start != -1) {
        char* word = copy_substring(expression, start, idx);
        if(is_operand(word)){
            add_token(&t, make_operand_token(atoi(word)));
        }else add_token(&t, make_token(word, WORD, NULL));
    }

    add_token(&t, make_end_token());
    return t;
    }

void print_token_value(Token* t){
    printf("%s : %s\n", t->expression, TokenTypeArr[t->tag]);
}

void print_Token(Token* t, bool print_next){
    if(t == NULL) return;
    switch(t->tag){
        case OPERATOR:
            printf("%c", t->operator);
            break;
        case WORD:
            printf("%s", t->expression);
            break;
        case NUMBER:
            printf("%d", t->value);
            break;
        case NEW_LINE:
            printf("[NEW LINE]");
            break;
        case TAB:
            printf("[TAB]");
            break;
        case  ASSSIGNEMENT:
            printf("=");
            break;
        case END:
            printf("END TOKEN");
            break;
        default:
            printf("unknown token");
            break;
    }
    printf("\t%s\n", TokenTypeArr[t->tag]);
    if(print_next) print_Token(t->next, print_next);
}

void free_tokens(Token* t){
    if(t == NULL) return;
    // printf("free token\n");
    free_tokens(t->next);
    if (t->tag == WORD) free(t->expression);
    free(t);
}

void print_Stack(TokenStack* t){
    printf("\nPRINT STACK\n\n");
    print_Token(t->first, true);
    // printf("Tokens printed\n");
}


void evaluate_expression(char* expression){
    TokenStack t = tokenize_expression(expression);
    print_Stack(&t);
    free_tokens(t.first);
    printf("tokens freed");
}

// int main(int argc, char* argv){
    
//     char buffer[100];
//     char c = getchar();
//     int idx = 0;
//     while (c != EOF)
//     {
//         // printf("%c", c);
//         //todo replace 4 spaces with tab
//         buffer[idx++] = c;
//         c = getchar();
//     }
//     buffer[idx] = 0;
//     printf("\n");
//     // printf("%s\n", buffer);

//     // evaluate_expression("x = 15 \n 15 + x");
//     evaluate_expression(buffer); 
//     // print_sub_string(1, 4, "Hello");
//     // char* s = copy_substring("Hello World", 0, 4);
//     // printf("%s", s);
// }