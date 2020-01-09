#include "libmx.h"

void mx_pop_back(t_list **head) {
    t_list *p = NULL;
    
    if (head && *head)
        p = *head;
    if (p != 0) {
        if (p->next == 0) {
            free(p->data);
            p->data = NULL;
            free(*head);
            *head = 0;
        }
        else {
            while (p->next->next != 0)
                p = p->next;
            free(p->next->data);
            p->next->data = NULL;
            free(p->next);
            p->next = 0;
        }
    }
}
