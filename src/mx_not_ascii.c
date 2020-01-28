#include "ush.h"

static void left_right(char *chars, int *table,  char **comands) {
    unsigned char check;

    if (chars[2] == 67) { // RIGHT
        mx_clean_monitor_new(MAIN_STRING, table[2], table[3], comands[table[0]]);
        check = (unsigned char)comands[*table][table[2] - table[3]];
        while (check >> 6 == 2) {
            table[3]--;
            check = (unsigned char)comands[*table][table[2] - table[3]];
        }
        table[3]--;
    }
    else { // LEFT
        mx_clean_monitor_new(MAIN_STRING, table[2], table[3], comands[table[0]]);
        check = (unsigned char)comands[*table][table[2] - table[3] - 2];
        while (check >> 6 == 2) {
            table[3]++;
            check = (unsigned char)comands[*table][table[2] - table[3] - 2];
        }
        table[3]++;
    }
}

static void up_down(char *chars, int *table,  char **comands){
    if (chars[2] == 65) { // UP
        if (table[0] != table[1]) {
            mx_clean_monitor_new(MAIN_STRING, table[2], table[3], comands[table[0]]);
            table[0]++;
            table[3] = 0;
            table[2] = mx_strlen(comands[table[0]]) + 1;
        }
        else 
            chars[2] = 10;
    }
    else if (chars[2] == 66) { //DOWN
        if (table[0] != 0) {
            mx_clean_monitor_new(MAIN_STRING, table[2], table[3], comands[table[0]]);
            table[0]--;
            table[3] = 0;
            table[2] = mx_strlen(comands[table[0]]) + 1;
        }
        else 
            chars[2] = 10;
    }
}

static void home_end(char *chars, int *table,  char **comands) {
    if (chars[2] == 70) { // position 0
        if (table[3] != 0) {
            mx_clean_monitor_new(MAIN_STRING, table[2], table[3], comands[table[0]]);
            table[3] = 0;
        }
        else 
            chars[2] = 10;
    }
    else if (chars[2] == 72) { // position end
        if (table[3] != table[2] - 1) {
            mx_clean_monitor_new(MAIN_STRING, table[2], table[3], comands[table[0]]);
            table[3] = table[2] - 1;
        }
        else 
            chars[2] = 10;
    }
    else 
        mx_printstr("/n/nEROOR!! DUBLE_COMAND\n\n");
}

static void pageU_PageD(char *chars, int *table, char **comands) {
    if (chars[2] == 53) { // PageUP
        if (table[0] != table[1]) {
            mx_clean_monitor_new(MAIN_STRING, table[2], table[3], comands[table[0]]);
            table[3] = 0;
            table[2] = mx_strlen(comands[table[1]]) + 1;
            table[0] = table[1];
        }
        else
            chars[2] = 10;
    }
    else if (chars[2] == 54) { //PageDown
        if (table[0] != 0) {
            mx_clean_monitor_new(MAIN_STRING, table[2], table[3], comands[table[0]]);
            table[2] = mx_strlen(comands[0]) + 1;
            table[3] = 0;
            table[0] = 0;
        }
        else
            chars[2] = 10;
    }
}

void mx_not_ascii(char *chars, int *table,  char **comands) {
    if (chars[3] == 0 && (chars[2] == 65 || chars[2] == 66)) {
        up_down(chars, table, comands);
    }
    else if (chars[3] == 0 && (chars[2] == 70 || chars[2] == 72)) {
        home_end(chars, table, comands);
    }
    else if (chars[3] == 0 && chars[2] == 68 && table[2] - 1 > table[3]) {
        left_right(chars, table, comands);
    }
    else if (chars[3] == 0 && chars[2] == 67 && 0 < table[3]) {
        left_right(chars, table, comands);
    }
    else if (chars[3] == 126 && (chars[2] == 53 || chars[2] == 54)) {
        pageU_PageD(chars, table, comands);
    }
    else if (chars[3] == 126 && chars[2] == 51 && table[3] != 0) {
        mx_key_delite(comands, table);
    }
    else
        chars[2] = 10;
}
