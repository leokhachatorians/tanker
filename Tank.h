#pragma once

class Tank {
    public:
        Tank(int row, int col);
        //~Tank();
        int get_row() const;
        int get_col() const;
        void set_row(int row);
        void set_col(int col);
        bool is_alive() const;
    
        void take_damage();
        void move();
        void shoot();

    private:
        int m_health;
        bool m_alive;
        int m_row;
        int m_col;
        char symbol;
};
