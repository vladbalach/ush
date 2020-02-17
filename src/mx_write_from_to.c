#include "ush.h"

void mx_write_from_to(int from , int to, off_t start) {
    char buff[1000];
    int c = 0;

    lseek(from, start, SEEK_SET);
    while ((c = read(from, buff, 1000))) {
        write(to, buff, c);
    }
}
