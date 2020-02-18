// cedit.c

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>

int EDIT_FD = STDOUT_FILENO; // in case you want to change it?
struct termios user_settings;

const char* CLEAR_SCREEN = "\x1b[2J";
const char* RESET_CURSOR = "\x1b[H";

// http://www.cs.uleth.ca/~holzmann/C/system/ttyraw.c
// found this to help w/ termios shtuff
void enter_input_mode() {
    struct termios raw;

    // grab copy of user terminal settings
    tcgetattr(EDIT_FD, &user_settings);

    // basically we want to set STDIN input to be rawest of the raw
    tcgetattr(EDIT_FD, &raw);

    // Setting flags to get rid of stuff like text echoing, signals, /r/n
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_iflag &= ~(ICRNL | IXON);
    raw.c_oflag &= ~(OPOST); 


    tcsetattr(EDIT_FD, TCSAFLUSH, &raw);
}

void reset_input_mode() {
    tcsetattr(EDIT_FD, TCSAFLUSH, &user_settings);
}

void fresh_screen() {
    write(EDIT_FD, CLEAR_SCREEN, strlen(CLEAR_SCREEN));
    write(EDIT_FD, RESET_CURSOR, strlen(RESET_CURSOR));
}

int main(int argc, char** argv) {
    char c;

    enter_input_mode();
    atexit(reset_input_mode);

    fresh_screen();

    while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q') {
        switch (c) {
            case '\r':
                write(EDIT_FD, "\r\n", 2);
                break;
            default:
                write(EDIT_FD, &c, 1); 
        }
        //fflush(stdout);
    }

    return 0;
}
