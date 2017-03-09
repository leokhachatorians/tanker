#include "Screen.h"
#include "Simplex.h"
#include <stdio.h>

Screen::Screen() {

    initscr();
    clear();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);
    getmaxyx(stdscr, _height, _width);
    double noise;
    double scale = 0.04;
    Simplex s = Simplex();

    for (int r = 1; r < _height - 2; r++) {
        for (int c = 2; c < _width; c++) {
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
    mvprintw(_height - 2, 10, "hello");

}

Screen::~Screen() {
    endwin();
}

void Screen::add_char(int row, int col, char x) {
    mvaddch(row, col, x);
}

int Screen::height() const {
    return _height;
}

int Screen::width() const {
    return _width;
}
