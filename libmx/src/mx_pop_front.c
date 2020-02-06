#include "libmx.h"

void mx_pop_front(t_list **head) {
if ((*head == 0) || (head == 0)) return;
t_list *tmp = (*head)->next;
    free(*head);
    *head = tmp;
}
