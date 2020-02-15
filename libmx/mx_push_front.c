#include "libmx.h"

void mx_push_front(t_list **list, void *data) {
    t_list *p = mx_create_node(data);
    t_list *s = NULL;

    if (list)
        s = *list;
    if (s != 0)
        p->next = s;
    *list = p;  
}
