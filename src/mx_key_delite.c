#include "ush.h"

void mx_key_delite(char **comands, int *table) {
    int i;
    int sum = mx_bit_sumbol(&comands[*table][table[2] - table[3] - 1]);

    mx_clean_monitor_new(NAME, table[2], table[3], comands[table[0]]);
    for (;sum > 0; sum--) {
        i = table[3];
        table[3]--;
        while (i > 0) {
            comands[*table][table[2] - i - 1] = comands[*table][table[2] - i];
            i--;
        }
        table[2]--;
    }
    comands[table[0]][table[2] - 1] = 0;
}
