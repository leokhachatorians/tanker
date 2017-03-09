#include "Screen.h"
#include "Simplex.h"
#include <stdio.h>
#include <ncurses.h>
#include <ctime>
#include <cstdlib>

Screen::Screen() {

    initscr();
    clear();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);
    getmaxyx(stdscr, _height, _width);
    double noise;
    double scale = 0.01;
    Simplex s = Simplex();
    std::srand(std::time(0));
    int ra = 1 + (std::rand() % (int)(5000));

    for (int r = 1; r < _height - 2; r++) {
        for (int c = 2; c < _width; c++) {
            //noise = s.simplex_noise((r + ra) * scale, (c + ra) * scale);
            noise = s.brownian(16, r+ra, c+ra, .5, scale, 0, 255);
            if (noise < 93) {
                mvaddch(r, c, '~');
            }
            else if (noise > 90 && noise < 100) {
                mvaddch(r, c, '=');
            }
            else if (noise > 100 && noise < 110) {
                mvaddch(r, c, 'S');
            }
            else if (noise > 110 && noise < 120) {
                mvaddch(r, c, '#');
            }
            else {
                mvaddch(r, c, '.');
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
