#include "libmx.h"

int mx_binary_search(char **arr, int size, const char *s, int *count) {
    int min = 0;
    int max = size - 1;
    int a;

    for (int i = 1; i <= size; i++) {
        a = (max + min) / 2;
        if (mx_strcmp(arr[a], s) == 0) {
            *count = i;
            return a;
        }
        if (max == min)
            return - 1;
        if (mx_strcmp(arr[a], s) < 0)
            min = a + 1;
        else
            max = a - 1;
    }
    return - 1;
}
