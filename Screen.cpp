#include "Screen.h"
#include "Simplex.h"
#include <stdio.h>
#include <ncurses.h>
#include <unistd.h>
#include <sys/ioctl.h>

Screen::Screen() {

    initscr();
    clear();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);
    // create and populate screen
    struct winsize w;
    double noise;
    double scale = 0.04;
    Simplex s = Simplex();
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    for (int r = 1; r < w.ws_row - 2; r++) {
        for (int c = 2; c < w.ws_col - 1; c++) {
            noise = s.noise(r * scale, c * scale);
            if (noise < .3) {
                mvaddch(r, c, '.');
            }
            else if (noise > .3 && noise < .5) {
                mvaddch(r, c, '#');
            }
            else if (noise > .5 && noise < .75) {
                mvaddch(r, c, 'S');
            }
            else {
                mvaddch(r, c, '~');
            }
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
