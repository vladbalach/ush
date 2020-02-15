#include "libmx.h"

int mx_list_size(t_list *list) {
    int i = 0;
    t_list *a;

    if (list != 0) {
        i++;
        a  = list;
        while (a->next != 0) {
            i++;
            a = a->next;
        }
    }
    return i;
}
