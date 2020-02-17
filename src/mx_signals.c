#include "ush.h"

void mx_segfault() {
    static int counter = 0;

    if (counter == 0) {
        mx_printerr("ush: segmentation fault\n");
        mx_print_unicode(0x1f92d);
        mx_printstr("\n");
    }
    if (counter == 1)
        mx_printerr("ush: still segmentation fault\n");
    if (counter == 2)
        mx_printerr("ush: dude, you have SEG FAULT!\n");
    if (counter == 3)
        mx_printerr("ush: keep trying!\n");
    if (counter == 4)
        mx_printerr("ush: can u compile that again?\n");
    if (counter == 5) {
        mx_printerr("ush: maybe programming not for you?\n");
        counter = -1;
    }
    counter++;
}

void mx_segfault_in() {
    exit(1);
}
void mx_ctrl_c() {
}

void mx_ctrl_z() {
}
