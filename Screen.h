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
        void adjust_scale(char action, double value);
        void adjust_iterations(char action, int value);
        void adjust_persistence(char action, double value);
    private:
        int _height;
        int _width;
        int _iterations;
        double _scale;
        double _persistence;
};
