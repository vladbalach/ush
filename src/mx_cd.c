#include "ush.h"

// return -1 on error
static int set_pwd() {
    char *buf = 0;
    char *pwd = 0;
    static char *temp = 0;

    if((buf = getcwd(NULL, 0)) == NULL) {
        return -1;
    }
    
    pwd = mx_strjoin("PWD=", buf);
    free(buf);
    if (temp != 0)
        free(temp);
    temp = pwd;
    putenv(pwd);
    return 0;
}

static int set_oldpwd() {
    char *buf = 0;
    char *pwd = 0;
    char *old_pwd = 0;
    static char *temp = 0;

    buf = mx_strdup(getenv("PWD"));
    pwd = mx_strjoin("PWD=", buf);
    old_pwd = mx_strjoin("OLD", pwd);
    free(buf);
    free(pwd);
    if (temp != 0)
        free(temp);
    temp = old_pwd;
    putenv(old_pwd);
    return 0;
}

void mx_cd(char *str[]) {
    char *str_ = str[1];
    if (str_ == NULL)
        str_ = getenv("HOME");
    if (mx_strcmp(str_, "-") == 0)
        str_ = getenv("OLDPWD");
    if ((chdir(str_) == -1) || (set_oldpwd() == -1) || (set_pwd() == -1)) {
        write(2,"cd: ",4);
        write(2, strerror(errno), mx_strlen(strerror(errno)));
        write(2,": ",2);
        write(2, str_, mx_strlen(str_));
        write(2,"\n",1);
        return;
    }
}
