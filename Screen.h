#pragma once

class Screen {
    public:
        Screen();
        ~Screen();
        void add_char(int row, int col, char x);
        int height() const;
        int width() const;
    private:
        int screen_height;
        int screen_width;
        int playable_width;
        int playable_height;
};
