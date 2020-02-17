#include "ush.h"

char **mx_call_vlad(char **argv, int i) {
    int counter = 0;
    char **argv_to_proces = NULL;

    for (; argv[i]; i++, counter++);
    argv_to_proces = malloc(sizeof(char *) * (counter + 1));
    argv_to_proces[counter] = NULL;
    for (i -= counter, counter = 0; argv[i]; i++, counter++)
        argv_to_proces[counter] = strdup(argv[i]);
    return argv_to_proces;
}
