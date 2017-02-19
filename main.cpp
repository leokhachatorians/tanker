#include <string>
#include <iostream>

const int MAXTANKS = 5;
const int MAXROWS = 30;
const int MAXCOLS = 30;
const int MAXSHOTLEN = 12;

enum Direction{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class Player {
    public:
        Player(int row, int col);
        //~Player();
        bool is_alive() const;
        int get_row() const;
        int get_col() const;
        int get_kills() const;
        int get_health() const;

        void move(Direction);
        void shoot(Direction);
        void add_kill(int kill);
        void take_damage();

    private:
        int m_health;
        int m_kills;
        int m_row;
        int m_col;
        bool m_alive;
};

Player::Player(int row, int col) {
    m_health = 2;
    m_kills = 0;
    m_row = row;
    m_col = col;
    m_alive = true;
}

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

void Player::take_damage() {
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
    public:
        Tank(int row, int col);
        ~Tank();
        int get_row() const;
        int get_col() const;
        bool is_alive() const;
    
        void take_damage();
        void move(Direction);
        void shoot(Direction);

    private:
        int m_health;
        bool m_alive;
        int m_row;
        int m_col;
};

int Tank::get_row() const {
    return m_row;
}

int Tank::get_col() const {
    return m_col;
}

bool Tank::is_alive() const {
    return m_alive;
}



class Arena {
    public:
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

    char grid[MAXROWS][MAXCOLS];
    Player p = Player(0, 0);

    for (int r = 0; r < MAXROWS; r++) {
        for (int c = 0; c < MAXCOLS; c++) {
            grid[r][c] = '.';
        }
    }

    grid[p.get_row()][p.get_col()] = 'P';

    for (int r = 0; r < MAXROWS; r++) {
        for (int c = 0; c < MAXCOLS; c++) {
            std::cout << grid[r][c];
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;

    p.move(LEFT);
    p.move(RIGHT);
    p.move(UP);

    grid[p.get_row()][p.get_col()] = 'P';

    for (int r = 0; r < MAXROWS; r++) {
        for (int c = 0; c < MAXCOLS; c++) {
            std::cout << grid[r][c];
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;


    

}
