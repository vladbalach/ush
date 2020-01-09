#include "libmx.h"

void mx_pop_front(t_list **head) {
    t_list *s = NULL;
    t_list *t = NULL;

    if (head && *head)
        s = *head;
    if (s != 0) {
        s = s->next;
        t = *head;
        free(t->data);
        t->data = NULL;
        t->next = NULL;
        free(t);
        *head = s;
    }
}
