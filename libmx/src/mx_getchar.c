#include "libmx.h"
#include <termios.h>

static void setTerminalSettings() {
    struct termios newTerm;

    tcgetattr(STDIN_FILENO, &newTerm);
    newTerm.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP
                | INLCR | IGNCR | ICRNL | IXON);
    newTerm.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    newTerm.c_cflag &= ~(CSIZE | PARENB);
    newTerm.c_cflag |= CS8;
    newTerm.c_iflag &= IGNCR;
    tcsetattr(STDIN_FILENO, 0, &newTerm);
}

static void disableTerminal(struct termios oldTerm) {
    tcsetattr(STDIN_FILENO, 0, &oldTerm);
}

unsigned int mx_getchar() {
    unsigned int ch = 0;
    struct termios oldTerm;

    tcgetattr(STDIN_FILENO, &oldTerm);
    setTerminalSettings();
    if (read(0, &ch, 4) == 0) {
        return 0;
    }
    disableTerminal(oldTerm);
    return ch;
}
