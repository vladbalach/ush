#include "ush.h"

t_info* mx_get_info(t_info *info) {
    static t_info* info_ = 0;
    if (info == 0)
        return info_;
    else
        info_ = info;
    return info_;
}
