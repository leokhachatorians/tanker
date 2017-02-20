#include <stdio.h>
#include <unistd.h>
#include <string>
#include <iostream>

// linux terminal size thing
#include <sys/ioctl.h>

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
        Arena(int nRows, int nCols, int num_tanks, Player *p);
        //~Arena();
        void move_player(Direction dir);
        void display_game_board();

    private:
        Player *m_player;
        int m_nTanks;
        int m_rows;
        int m_cols;
        char m_grid[MAXROWS][MAXCOLS];
        Tank *m_ranks[MAXTANKS];
};

Arena::Arena(int rows, int cols, int num_tanks, Player *p) {
    m_rows = rows;
    m_cols = cols;
    m_nTanks = num_tanks;
    m_player = p;

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            m_grid[r][c] = '.';
        }
    }
    m_grid[p->get_row()][p->get_col()] = 'P';
}

void Arena::display_game_board() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    for (int i = 0; i < w.ws_row - m_rows; i++) {
        std::cout << std::endl;
    }

    for (int r = 0; r < m_rows; r++) {
        for (int c = 0; c < m_cols; c++) {
            std::cout << m_grid[r][c];
        }
        std::cout << std::endl;
    }
}

void Arena::move_player(Direction dir) {
    m_grid[m_player->get_row()][m_player->get_col()] = '.';
    m_player->move(dir);
    m_grid[m_player->get_row()][m_player->get_col()] = 'P';
}

class Game {
    public:
        Game(Player *p, Arena *a);
        void run();
    private:
        Player *g_player;
        Arena *g_arena;
        bool g_running;
        int count;
};

Game::Game(Player *p, Arena *a) {
    g_player = p;
    g_arena = a;
    g_running = true;
}

void Game::run() {
    count = 1;
    while (g_running == true) {
        g_arena->display_game_board();
        count += 1;
        if (count == 5) {
            g_running = false;
        }
    }
}
    


int main(int, char *[]) {
    Player player = Player(0, 0);
    Arena arena = Arena(30, 30, 10, &player);
    Game game = Game(&player, &arena);
    game.run();

    return 0;
}
