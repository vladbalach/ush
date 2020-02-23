#include "libmx.h"

char *mx_file_to_str(const char *file) {
    int desc = open(file, O_RDONLY);
    char c = 0;
    char *str = 0;
    size_t count = 0;
    size_t i = 0;

    if(desc < 0) 
        return 0;
    while (read(desc,&c,1))
        count++;
    close(desc);
    if((str = (char *)malloc(sizeof(char) * count + 1)) == 0)
        return 0;
    if((desc = open(file, O_RDONLY)) == 0) 
        return 0;
    while (read(desc, &c, 1))
        str[i++] = c;
    close(desc);
    str[count] = '\0';
    return str;
}
