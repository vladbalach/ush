#include "ush.h"

unsigned int mx_read_keyboard(t_info *info) {
    unsigned int ch = 0;

    if ((ch = mx_getchar()) == 0) {
        mx_printerr("u$h: some troubeles with input!\n");
        exit(2);
    }
    if (info->input->if_  == 9 && ch != 9)
        mx_del_strarr(&(info->input->comand_tab));
    return ch;
}
