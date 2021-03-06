#pragma once
#include "globals.h"

class Player {
    public:
        Player(int row, int col);
        //~Player();
        bool is_alive() const;
        bool is_shooting() const;
        int get_row() const;
        int get_col() const;
        int get_kills() const;
        int get_health() const;

        char get_sym() const;
        char get_shooting_dir() const;

        void set_pos(int row, int col);
        void set_shooting_dir(char);
        void set_is_shooting(bool value);
        void add_kill(int kill);
        void take_damage();

    private:
        int m_health;
        int m_kills;
        int m_row;
        int m_col;
        char m_shooting_dir;
        char m_symbol;
        bool m_alive;
        bool m_shooting;
        int m_shots;
};
