#include "libmx.h"

t_list *mx_sort_list(t_list *lst, bool (*cmp)(void *, void *)) {
    int i = mx_list_size(lst);
    t_list *s = lst;
    void *l = NULL;

    for (int a = 0; a < i; a++) {
        s = lst;
        if (lst != 0 || lst->next != 0) {
            while (s->next != 0) {
                if (cmp(s->data, s->next->data)) {
                    l = s->data;
                    s->data = s->next->data;
                    s->next->data = l;
                }
                s = s->next;
            }
        }
    }
    return lst;
}
