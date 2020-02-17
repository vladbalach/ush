#include "ush.h"

t_token *mx_create_token(char type, char **value, int priority) {
    t_token* newToken = (t_token*) malloc(sizeof(t_token));

    newToken->type = type;
    newToken->value = value;
    newToken->priority = priority;
    return newToken;
}
