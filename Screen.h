#include <ncurses.h>
#pragma once

class Screen {
    public:
        Screen();
        ~Screen();
        void add_char(int row, int col, char x);
        int height() const;
        int width() const;
    private:
        int _height;
        int _width;
};
