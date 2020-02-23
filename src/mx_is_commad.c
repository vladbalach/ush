#include "ush.h"

bool mx_is_commad(char *fullname, int flags) {
    struct stat st;

    if (stat(fullname, &st) != -1) { // finded
        if ((st.st_mode & S_IXUSR) == S_IXUSR) {
            if ((flags & 2) == 2) {
                free(fullname);
                return true;
            }  
            mx_printstr(fullname);
            mx_printchar('\n');
            free(fullname);
            return true;
        }
    }
    free(fullname);
    return false;
}
