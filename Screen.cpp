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
    _iterations = 16;
    _scale = 0.01;
    _persistence = 0.5;
    draw_world();
}

Screen::~Screen() {
    endwin();
}

void Screen::draw_world() {
    double noise;
    Simplex s = Simplex();
    std::srand(std::time(0));
    int ra = 1 + (std::rand() % (int)(5000));

    for (int r = 1; r < _height - 2; r++) {
        for (int c = 2; c < _width; c++) {
            //noise = s.simplex_noise((r + ra) * scale, (c + ra) * scale);
            noise = s.brownian(_iterations, r+ra, c+ra, _persistence, _scale, 0, 255);
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
    mvprintw(_height - 2, 10, "<R> Redraws map\t<+/-> Adjust Scale\t<Q> To Quit");

}

void Screen::add_char(int row, int col, char x) {
    mvaddch(row, col, x);
}

bool Screen::check_movement(int row, int col) {
    char check = mvinch(row, col) & A_CHARTEXT;
    if (check == '.') {
        return true;
    }
    return false;
}

int Screen::height() const {
    return _height;
}

int Screen::width() const {
    return _width;
}

void Screen::adjust_scale(char action, double value) {
    if (action == 'i') {
        _scale += value;
    }
    else {
        _scale -= value;
    }
}

void Screen::adjust_persistence(char action, double value) {
    if (action == 'i') {
        _persistence += value;
    }
    else {
        _persistence -= value;
    }
}

void Screen::adjust_iterations(char action, int value) {
    if (action == 'i') {
        _iterations += value;
    }
    else {
        _iterations -= value;
    }
}
