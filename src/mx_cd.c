#include "ush.h"

// return -1 on error
static int set_pwd() {
    char *buf = 0;
    char *pwd = 0;

    if((buf = getcwd(NULL, 0)) == NULL) {
        return -1;
    }
    free(buf);
    pwd = mx_strjoin("PWD=", buf);
    putenv(pwd);   
    return 0; 
}

static int set_oldpwd() {
    char *buf = 0;
    char *pwd = 0;
    char *old_pwd = 0;

    buf = mx_strdup(getenv("PWD"));
    pwd = mx_strjoin("PWD=", buf);
    old_pwd = mx_strjoin("OLD", pwd);
    free(buf);
    free(pwd);
    putenv(old_pwd); 
    return 0; 
}

void mx_cd(char *str) {
    if ((chdir(str) == -1) || (set_oldpwd() == -1) || (set_pwd() == -1)) {
        write(2,"cd: ",4);
        write(2, strerror(errno), mx_strlen(strerror(errno)));
        write(2,": ",2);
        write(2, str, mx_strlen(str));
        write(2,"\n",1);
        return;
    }
}
