#include <stdio.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <thread>

// linux terminal size thing
#include <sys/ioctl.h>

const int MAXTANKS = 10;
const int MAXROWS = 30;
const int MAXCOLS = 30;
const int MAXSHOTLEN = 12;

enum Direction{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    last
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
        //~Tank();
        int get_row() const;
        int get_col() const;
        bool is_alive() const;
    
        void take_damage();
        void move();
        void shoot();

    private:
        int m_health;
        bool m_alive;
        int m_row;
        int m_col;
};

Tank::Tank(int row, int col) {
    m_health = 2;
    m_alive = true;
    m_row = row;
    m_col = col;
}

int Tank::get_row() const {
    return m_row;
}

int Tank::get_col() const {
    return m_col;
}

bool Tank::is_alive() const {
    return m_alive;
}

void Tank::move() {
    Direction dir = static_cast<Direction>(rand() % last);
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

class Arena {
    public:
        Arena(int nRows, int nCols, int num_tanks, Player *p);
        //~Arena();
        void move_player(Direction dir);
        void move_tanks();
        void display_game_board();

    private:
        Player *m_player;
        int m_nTanks;
        int m_rows;
        int m_cols;
        char m_grid[MAXROWS][MAXCOLS];
        Tank *m_tanks[MAXTANKS];
};

Arena::Arena(int rows, int cols, int num_tanks, Player *p) {
    // sets up game board, places player position as well as
    // all AI tanks
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

    // init random seed
    srand(time(NULL));

    for (int i = 0; i < num_tanks; i++) {
        int row = rand() % rows;
        int col = rand() % cols;

        printf("row: %d\n", row);
        printf("col: %d\n", col);
        Tank *tank = new Tank(row, col);
        m_tanks[i] = tank;
        m_grid[row][col] = 'T';
    }
}

void Arena::display_game_board() {
    // Current structure takes into account terminal size
    // and adjusts how much blank space to display inbetween
    // each 'turn'. Later on i want to make it real time?
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
    // Replaces player position with blank space, moves player
    // and then sets player position accordingly
    m_grid[m_player->get_row()][m_player->get_col()] = '.';
    m_player->move(dir);
    m_grid[m_player->get_row()][m_player->get_col()] = 'P';
}

void Arena::move_tanks() {
    for (int i = 0; i < m_nTanks; i++) {
        int t_row = m_tanks[i]->get_row();
        int t_col = m_tanks[i]->get_col();
        m_grid[t_row][t_col] = '.';

        m_tanks[i]->move();
        t_row = m_tanks[i]->get_row();
        t_col = m_tanks[i]->get_col();
        m_grid[t_row][t_col] = 'T';
    }
}

class Game {
    public:
        Game(Player *p, Arena *a);
        Game();
        void robo_thread();
        void player_thread();
        void run();
    private:
        Player *g_player;
        Arena *g_arena;
        bool g_running;
        char g_user_in;
        int count;
};

Game::Game(Player *p, Arena *a) {
    g_player = p;
    g_arena = a;
    g_running = true;
}

Game::Game() {}

void Game::run() {
    count = 0;
    std::thread r_thread(&Game::robo_thread, this);
    std::thread p_thread(&Game::player_thread, this);

    while (g_running == true) {
        g_arena->display_game_board();
        count += 1;
        if (count == 10) {
            g_running = false;
        }
        usleep(1000000);
    }
    r_thread.join();
    p_thread.join();
}

void Game::robo_thread() {
    while (g_running == true) {
        g_arena->move_tanks();
        usleep(1000000);
    }
}

void Game::player_thread() {
    while (g_running == true) {
        g_arena->move_player(RIGHT);
        //g_arena->display_game_board();
        usleep(1500000);
    }
}

int main(int, char *[]) {
    Player player = Player(0, 0);
    Arena arena = Arena(30, 30, 10, &player);
    Game game = Game(&player, &arena);
    game.run();

    return 0;
}
