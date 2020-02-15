#include "libmx.h"

int mx_bubble_sort(char **arr, int size) {
    int sum = 0;
    char *save;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size - 1; j++) {
            if (mx_strcmp(arr[j], arr[j + 1]) > 0) {
                save = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = save;
                sum++;
            }
        }
    }
    return sum;
}
