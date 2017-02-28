#include "Screen.h"
#include <stdio.h>
#include <ncurses.h>
#include <unistd.h>
#include <sys/ioctl.h>

Screen::Screen() {
    // create and populate screen
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    for (int r = 1; r < w.ws_row - 2; r++) {
        for (int c = 2; c < w.ws_col - 1; c++) {
            mvaddch(r, c, '.');
        }
    }

    screen_height = w.ws_row;
    screen_width = w.ws_col;

    mvprintw(w.ws_row - 2, 10, "hello");

}

Screen::~Screen() {
    endwin();
}

void Screen::add_char(int row, int col, char x) {
    mvaddch(row, col, x);
}

int Screen::height() const {
    return screen_height;
}

int Screen::width() const {
    return screen_width;
}
