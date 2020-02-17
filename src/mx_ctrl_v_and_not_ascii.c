#include "ush.h"

void mx_ctrl_v_and_not_ascii(t_info *info, char *chars) {
    if (chars[0] == 27)
        mx_not_ascii(chars, info);
    else {
        mx_clean_monitor_new(MX_NAME, MX_STR_LEN, MX_STR_POS, MX_STR);
        for (int i = 0; i < 4 && chars[i] != 0; i++)
            mx_one_symbol(&(MX_STR), chars[i], &(MX_STR_LEN), MX_STR_POS);
    }
}
