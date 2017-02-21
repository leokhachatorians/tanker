#pragma once
#include "globals.h"

class Player {
    public:
        Player(int row, int col);
        //~Player();
        bool is_alive() const;
        int get_row() const;
        int get_col() const;
        int get_kills() const;
        int get_health() const;
        char get_sym() const;

        void move(Direction);
        void shoot(Direction);
        void add_kill(int kill);
        void take_damage();

    private:
        int m_health;
        int m_kills;
        int m_row;
        int m_col;
        char m_symbol;
        bool m_alive;
};
