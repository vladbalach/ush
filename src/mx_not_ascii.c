#include "ush.h"

static void left_right(char *chars, t_info *info) {
    unsigned char check;

    if (chars[2] == 67) { // RIGHT
        mx_clean_monitor_new(MX_NAME, MX_STR_LEN, MX_STR_POS, MX_STR);
        check = (unsigned char)MX_STR[MX_STR_LEN - MX_STR_POS];
        while (check >> 6 == 2) {
            MX_STR_POS--;
            check = (unsigned char)MX_STR[MX_STR_LEN - MX_STR_POS];
        }
        (MX_STR_POS)--;
    }
    else { // LEFT
        mx_clean_monitor_new(MX_NAME, MX_STR_LEN, MX_STR_POS, MX_STR);
        check = (unsigned char)MX_STR[MX_STR_LEN - MX_STR_POS - 2];
        while (check >> 6 == 2) {
            (MX_STR_POS)++;
            check = (unsigned char)MX_STR[MX_STR_LEN - MX_STR_POS - 2];
        }
        (MX_STR_POS)++;
    }
}

static void up_down(char *chars, t_info *info){
    if (chars[2] == 65) { // UP
        if (MX_ID != MX_MAX_COMAND) {
            mx_clean_monitor_new(MX_NAME, MX_STR_LEN, MX_STR_POS, MX_STR);
            (MX_ID)++;
            MX_STR_POS = 0;
            MX_STR_LEN = mx_strlen(MX_STR) + 1;
        }
        else 
            chars[2] = 10;
    }
    else if (chars[2] == 66) { //DOWN
        if (MX_ID != 0) {
            mx_clean_monitor_new(MX_NAME, MX_STR_LEN, MX_STR_POS, MX_STR);
            (MX_ID)--;
            MX_STR_POS = 0;
            MX_STR_LEN = mx_strlen(MX_STR) + 1;
        }
        else 
            chars[2] = 10;
    }
}

static void home_end(char *chars, t_info *info) {
    if (chars[2] == 70) { // position 0
        if (MX_STR_POS != 0) {
            mx_clean_monitor_new(MX_NAME, MX_STR_LEN, MX_STR_POS, MX_STR);
            MX_STR_POS = 0;
        }
        else 
            chars[2] = 10;
    }
    else if (chars[2] == 72) { // position end
        if (MX_STR_POS != MX_STR_LEN - 1) {
            mx_clean_monitor_new(MX_NAME, MX_STR_LEN, MX_STR_POS, MX_STR);
            MX_STR_POS = MX_STR_LEN - 1;
        }
        else 
            chars[2] = 10;
    }
    else 
        mx_printstr("/n/nEROOR!! DUBLE_COMAND\n\n");
}

static void pageU_PageD(char *chars, t_info *info) {
    if (chars[2] == 53) { // PageUP
        if (MX_ID != MX_MAX_COMAND) {
            mx_clean_monitor_new(MX_NAME, MX_STR_LEN, MX_STR_POS, MX_STR);
            MX_STR_POS = 0;
            MX_STR_LEN = mx_strlen(info->input->comands[MX_MAX_COMAND]) + 1;
            MX_ID = MX_MAX_COMAND;
        }
        else
            chars[2] = 10;
    }
    else if (chars[2] == 54) { //PageDown
        if (MX_ID != 0) {
            mx_clean_monitor_new(MX_NAME, MX_STR_LEN, MX_STR_POS, MX_STR);
            MX_STR_LEN = mx_strlen(info->input->comands[0]) + 1;
            MX_STR_POS = 0;
            MX_ID = 0;
        }
        else
            chars[2] = 10;
    }
}

void mx_not_ascii(char *chars, t_info *info) {
    if (chars[3] == 0 && (chars[2] == 65 || chars[2] == 66)) {
        up_down(chars, info);
    }
    else if (chars[3] == 0 && (chars[2] == 70 || chars[2] == 72)) {
        home_end(chars, info);
    }
    else if (chars[3] == 0 && chars[2] == 68 && MX_STR_LEN - 1 > MX_STR_POS) {
        left_right(chars, info);
    }
    else if (chars[3] == 0 && chars[2] == 67 && 0 < MX_STR_POS) {
        left_right(chars, info);
    }
    else if (chars[3] == 126 && (chars[2] == 53 || chars[2] == 54)) {
        pageU_PageD(chars, info);
    }
    else if (chars[3] == 126 && chars[2] == 51 && MX_STR_POS != 0) {
        mx_key_delite(info);
    }
    else
        chars[2] = 10;
}
