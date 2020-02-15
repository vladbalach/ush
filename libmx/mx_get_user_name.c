#include "libmx.h"

char *mx_get_user_name() {
    uid_t uid =  getuid();
    struct passwd *psw = getpwuid(uid);
    return psw->pw_name;
}
