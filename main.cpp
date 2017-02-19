#include <string>
#include <iostream>

const int MAXTANKS = 100;
const int MAXROWS = 20;
const int MAXCOLS = 20;
const int MAXSHOTLEN = 5;

enum Direction{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class Player {
    Player(int row, int col);
    ~Player();

    public:
        bool is_alive() const;
        int get_row() const;
        int get_col() const;
        int get_kills() const;
        int get_health() const;

        void move(Direction);
        void add_kill(int kill);
        void reduce_health();

    private:
        int m_health;
        int m_kills;
        bool alive;
};

class Tank {
    Tank(int row, int col);
    ~Tank();
    
    public:
        int get_row() const;
        int get_col() const;;
        int get_health() const;

        void move(Direction);

    private:
        int m_health;
        bool alive;

};

class Arena {
    Arena(int nRows, int nCols);
    ~Arena();

    private:
        Player *m_player;
        int m_nTanks;
        int m_rows;
        int m_cols;
        Tank *m_ranks[MAXTANKS];
};


int main(int, char *[]) {
}
