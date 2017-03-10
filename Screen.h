#include <ncurses.h>
#pragma once

class Screen {
    public:
        Screen();
        ~Screen();
        void add_char(int row, int col, char x);
        void draw_world();
        int height() const;
        int width() const;
        bool check_movement(int r, int c);
    private:
        int _height;
        int _width;
        WINDOW *_window;
};
