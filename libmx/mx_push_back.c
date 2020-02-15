#include "libmx.h"

void mx_push_back(t_list **list, void *data) {
    t_list *p = mx_create_node(data);
    t_list *s = NULL;

    if (list)
        s = *list;
    if (s != 0) {
        while(s->next != 0)
            s = s->next;
        s->next = p;
    }
    else
        *list = p; 
}
