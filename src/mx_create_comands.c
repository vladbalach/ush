#include "ush.h"
static int sum_comands(char *str, int end) {
    int sum = 0;

    for(int i = 0; i <= end; i ++)
        if (str[i] == 0 && i != 0 && i != end - 1)
            sum++;
    return sum;
}

static char *create_comand(char *str, int *position) {
    char *comand = mx_strdup(str);
    int len = mx_strlen(comand);

    *position = *position + len + 1;
    return comand;
}

char **mx_create_comands(char *str, int end) {
    int sum = sum_comands(str, end);
    char **comands = (char **) malloc (sizeof(char *) * (sum + 1));
    int position = 0;

    for (int i = 0; position < end && i < sum; i++)
        comands[i] = create_comand(&str[position], &position);
    comands[sum] = 0;

    // mx_printstr("\n");
    // for (int i = 0; comands[i]; i++) {
    //     mx_printstr(comands[i]);
    //     mx_printstr("|\n");
    // }
    // mx_printstr("\n\n");
    // mx_printint(sum);
    // mx_printstr("\n\n");
    return comands;
}
