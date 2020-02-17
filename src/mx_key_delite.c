#include "ush.h"

void mx_key_delite(t_info *info) {
    int i;
    int sum = mx_bit_sumbol(&MX_STR[MX_STR_LEN - MX_STR_POS - 1]);

    mx_clean_monitor_new(MX_NAME, MX_STR_LEN, MX_STR_POS, MX_STR);
    for (;sum > 0; sum--) {
        i = MX_STR_POS;
        MX_STR_POS--;
        while (i > 0) {
            MX_STR[MX_STR_LEN - i - 1] = MX_STR[MX_STR_LEN - i];
            i--;
        }
        MX_STR_LEN--;
    }
    info->input->comands[MX_MAX_COMAND][MX_STR_LEN - 1] = 0;
}
