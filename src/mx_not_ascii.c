#include "ush.h"

static void dual_comands(char *name, char *chars, int *table,  char **comands) {
    unsigned char check;

    if (chars[2] == 65) { // UP
        if (table[0] != table[1]) {
            mx_clean_monitor_new(name, table[2], table[3], comands[table[0]++]);
            table[3] = 0;
            table[2] = mx_strlen(comands[table[0]]) + 1;
        }
        else 
            chars[2] = 10;
    }
    else if (chars[2] == 70) { // position 0
        if (table[3] != 0) {
            mx_clean_monitor_new(name, table[2], table[3], comands[table[0]]);
            table[3] = 0;
        }
        else 
            chars[2] = 10;
    }
    else if (chars[2] == 72) { // position end
        if (table[3] != table[2] - 1) {
            mx_clean_monitor_new(name, table[2], table[3], comands[table[0]]);
            table[3] = table[2] - 1;
        }
        else 
            chars[2] = 10;
    }
    else if (chars[2] == 66) { //DOWN
        if (table[0] != 0) {
            mx_clean_monitor_new(name, table[2], table[3], comands[table[0]--]);
            table[3] = 0;
            table[2] = mx_strlen(comands[table[0]]) + 1;
        }
        else 
            chars[2] = 10;
    }
    else if (chars[2] == 67) { // RIGHT
        if (0 < table[3]) {
            mx_clean_monitor_new(name, table[2], table[3], comands[table[0]]);
            check = (unsigned char)comands[*table][table[2] - table[3]];
            while (check >> 6 == 2) {
                table[3]--;
                check = (unsigned char)comands[*table][table[2] - table[3]];
            }
            table[3]--;
        }
        else 
            chars[2] = 10;
    }
    else if (chars[2] == 68) { // LEFT
        if (table[2] - 1 > table[3]) {
            mx_clean_monitor_new(name, table[2], table[3], comands[table[0]]);
            check = (unsigned char)comands[*table][table[2] - table[3] - 2];
            while (check >> 6 == 2) {
                table[3]++;
                check = (unsigned char)comands[*table][table[2] - table[3] - 2];
            }
            table[3]++;
        }
        else
            chars[2] = 10;
    }
    else 
        mx_printstr("/n/nEROOR!! DUBLE_COMAND\n\n");
}

static void key_delite(char *name, char **comands, int *table) {
    int i;
    int sum = mx_bit_sumbol(&comands[*table][table[2] - table[3] - 1]);

    mx_clean_monitor_new(name, table[2], table[3], comands[table[0]]);
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

static void three_comands(char *name, char *chars, int *table, char **comands) {
    if (chars[2] == 53) { // PageUP
        if (table[0] != table[1]) {
            mx_clean_monitor_new(name, table[2], table[3], comands[table[0]]);
            table[3] = 0;
            table[2] = mx_strlen(comands[table[1]]) + 1;
            table[0] = table[1];
        }
        else
            chars[2] = 10;
    }
    else if (chars[2] == 54) { //PageDown
        if (table[0] != 0) {
            mx_clean_monitor_new(name, table[2], table[3], comands[table[0]]);
            table[2] = mx_strlen(comands[0]) + 1;
            table[3] = 0;
            table[0] = 0;
        }
        else
            chars[2] = 10;
    }
    else if (chars[2] == 51) { //delete
        if (table[3] != 0)
            key_delite(name, comands, table);
        else
            chars[2] = 10;
    }
    else 
        mx_printstr("/n/nEROOR!! THREE_COMAND\n\n");
}

void mx_not_ascii(char *name, char *chars, int *table,  char **comands) {
    if (chars[3] == 0 && chars[2] >= 65 && chars[2] <= 68) {
        dual_comands(name, chars, table, comands);
    }
    else if (chars[3] == 0 && (chars[2] == 70 || chars[2] == 72)) {
        dual_comands(name, chars, table, comands);
    }
    else if (chars[3] == 126 && (chars[2] == 51 || chars[2] == 54)) {
        three_comands(name, chars, table, comands);
    }
    else if (chars[3] == 126 && chars[2] == 53) {
        three_comands(name, chars, table, comands);
    }
    else
        chars[2] = 10;
}