#include "libmx.h"

static void setTerminalSettings() {
    t_termios newTerm;
    tcgetattr(STDIN_FILENO, &newTerm);
    newTerm.c_lflag &= ~(ICANON | ECHO);
    newTerm.c_lflag &= ~(ISIG);
    // newTerm.c_iflag &= ~INPCK;
    newTerm.c_iflag &= IGNCR; // IGNOR NEWLINE (return 13 on return)
    tcsetattr(STDIN_FILENO, 0, &newTerm);
}

static void disableTerminal(t_termios oldTerm) {
    tcsetattr(STDIN_FILENO, 0, &oldTerm);
}

// get char immediately
unsigned int mx_getchar() {
    unsigned int ch = 0;
    t_termios oldTerm;

    tcgetattr(STDIN_FILENO, &oldTerm);
    setTerminalSettings();
    if (read(1, &ch, 3) == 0) {
        return 0;
    }
    disableTerminal(oldTerm);
    return ch;
}
