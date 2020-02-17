#include "ush.h"

static void not_id_0(char **str, char **comands, t_info *info) {
    for (int i = 0; comands[MX_ID_TAB_KEY - 1][i]; i++)
        mx_one_symbol(str, 127, &(MX_STR_LEN), MX_STR_POS);
    if (comands[MX_ID_TAB_KEY] != 0) {
        for (int i = 0; comands[MX_ID_TAB_KEY][i]; i++)
            mx_one_symbol(str, comands[MX_ID_TAB_KEY][i],
                          &(MX_STR_LEN), MX_STR_POS);
        MX_ID_TAB_KEY++;
    }
    else {
        for (int i = 0; comands[0][i]; i++)
            mx_one_symbol(str, comands[0][i], &(MX_STR_LEN), MX_STR_POS);
        MX_ID_TAB_KEY = 1;
    }
}

void mx_key_duble_tab(char **str, char **comands, t_info *info) {
    if (comands != 0 && comands[0] != 0) {
        if (MX_ID_TAB_KEY == 0 && comands[0] != 0) {
            for (int i = 0; comands[0][i]; i++)
                mx_one_symbol(str, comands[0][i], &(MX_STR_LEN), MX_STR_POS);
            if (MX_STR_POS != 0) {
                mx_one_symbol(str, ' ', &(MX_STR_LEN), MX_STR_POS);
                MX_STR_POS++;
            }
            MX_ID_TAB_KEY++;
        }
        else
            not_id_0(str,comands, info);
    }
    else
        write(1,"\a", 1);
}
