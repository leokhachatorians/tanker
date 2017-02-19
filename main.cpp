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
        int m_row;
        int m_col;
        bool m_alive;
};

int Player::get_row() const {
    return m_row;
}

int Player::get_col() const {
    return m_col;
}

int Player::get_kills() const {
    return m_kills;
}

int Player::get_health() const {
    return m_health;
}

bool Player::is_alive() const {
    return m_alive;;
}

void Player::add_kill(int amount) {
    m_kills += amount;
}

void Player::reduce_health() {
    m_health -= 1;
    if (m_health == 0) {
        m_alive = false;
    }
}

void Player::move(Direction dir) {
    if (dir == UP && m_row > 1) {
        m_row -= 1;
    }
    else if (dir == DOWN && m_row < MAXROWS - 1) {
        m_row += 1;
    }
    else if (dir == LEFT && m_col > 1) {
        m_col -= 1;
    }
    else if (dir == RIGHT && m_col < MAXCOLS - 1) {
        m_col += 1;
    }
}




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
