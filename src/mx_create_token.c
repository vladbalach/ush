#include "ush.h"

t_token *mx_create_token(char type, char **value, int priority) {
    t_token* new_token = (t_token*) malloc(sizeof(t_token));

    new_token->type = type;
    new_token->value = value;
    new_token->priority = priority;
    return new_token;
}
