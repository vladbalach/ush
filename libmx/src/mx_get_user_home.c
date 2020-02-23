#include "libmx.h"

char *mx_get_user_home() {
    uid_t uid =  getuid();
    struct passwd *psw = getpwuid(uid);

    return psw->pw_dir;
}
