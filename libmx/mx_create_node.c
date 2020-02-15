#include "libmx.h"

t_list *mx_create_node(void *data) {
    t_list *p = malloc(sizeof(t_list));

    p->data = data;
    p->next = NULL;
    return p;
}
