#include "libmx.h"

static bool del(t_list *s, t_list **head, void(*del_data)(void *)) {
    del_data(s->data);
    mx_pop_front_free_data(head);
    return 1;
}

void mx_pop_list(t_list **head, void *data, bool(*if_list)(void *, void *),
                 void(*del_data)(void *)) {
    t_list *s = NULL;
    t_list *temp = NULL;

    if (head == 0 || *head == 0)
        return;
    s = *head;
    if (if_list(data, s->data) && del(s, head, del_data))
        return;
    while (s->next != 0) {
        if (if_list(data, s->next->data)) {
            del_data(s->next->data);
            temp = s->next;
            free(temp->data);
            s->next = temp->next;
            free(temp);
            return;
        } 
        s = s->next;
    }
}
