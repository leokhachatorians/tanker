#include <stdio.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <thread>
#include <ncurses.h>

// linux terminal size thing
#include <sys/ioctl.h>

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

Screen::Screen() {
    // create and populate screen
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    for (int r = 1; r < w.ws_row - 2; r++) {
        for (int c = 2; c < w.ws_col - 1; c++) {
            mvaddch(r, c, '.');
        }
    }

    screen_height = w.ws_row;
    screen_width = w.ws_col;

}

Screen::~Screen() {
    endwin();
}

void Screen::add_char(int row, int col, char x) {
    mvaddch(row, col, x);
}

int Screen::height() const {
    return screen_height;
}

int Screen::width() const {
    return screen_width;
}

void init_curses();
void game_loop(int, Screen);
int generate_random_pos(Screen);

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

Player::Player(int row, int col) {
    m_health = 2;
    m_kills = 0;
    m_row = row;
    m_col = col;
    m_alive = true;
    m_symbol = '@';
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

char Player::get_sym() const {
    return m_symbol;
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

Tank::Tank(int row, int col) {
    m_health = 2;
    m_alive = true;
    m_row = row;
    m_col = col;
    symbol = 'T';
}

int Tank::get_row() const {
    return m_row;
}

int Tank::get_col() const {
    return m_col;
}

void Tank::set_row(int row) {
    m_row = row;
}

void Tank::set_col(int col) {
    m_col = col;
}

bool Tank::is_alive() const {
    return m_alive;
}

class Game {
    public:
        Game(Player *p, Screen *s, int num_tanks);
        void init_robo_tanks();
        void robo_thread();
        void player_thread();
        void run();
        int generate_random_pos();
    private:
        Player  *g_player;
        Screen  *g_screen;
        bool     g_running;
        int      m_num_tanks;
        Tank    *m_tanks[MAXTANKS];
};

Game::Game(Player *p, Screen *s, int num_tanks) {
    // Create game class, and place player character on screen
    g_player = p;
    g_screen = s;
    m_num_tanks = num_tanks;
    g_running = true;
    mvaddch(p->get_row(), p->get_col(), p->get_sym());
    init_robo_tanks();
}

int Game::generate_random_pos() {
    int pos = rand() % (g_screen->height() - 3);
    return pos;
}

void Game::init_robo_tanks() {
    for (int i = 0; i < m_num_tanks; i++) {
        int row = rand() % (g_screen->height() - 3);
        int col = rand() % (g_screen->width() - 3);

        Tank *tank = new Tank(row, col);
        m_tanks[i] = tank;
        mvaddch(row, col, 'T');
    }
}

void Game::player_thread() {
    int row = g_player->get_row();
    int col = g_player->get_col();
    int ch = getch();

    for(;;) {
        ch = getch();
        if (ch == KEY_LEFT && col > 2) {
            mvaddch(row, col, '.');
            col -= 1;
            mvaddch(row, col, g_player->get_sym());
            refresh();
        }
        else if (ch == KEY_RIGHT && col < g_screen->width() - 2) {
            mvaddch(row, col, '.');
            col += 1;
            mvaddch(row, col, g_player->get_sym());
            refresh();
        }
        else if (ch == KEY_UP && row > 1) {
            mvaddch(row, col, '.');
            row -= 1;
            mvaddch(row, col, g_player->get_sym());
            refresh();
        }
        else if (ch == KEY_DOWN && row < g_screen->height() - 3) {
            mvaddch(row, col, '.');
            row += 1;
            mvaddch(row, col, g_player->get_sym());
            refresh();
        }
        else if (ch == 'q' || ch == 'Q') {
            g_running = false;
            break;
        }
    }
}

void Game::robo_thread() {
    while (g_running) {
        for (int i = 0; i < m_num_tanks; i++) {
            Direction dir = static_cast<Direction>(rand() % last);
            int row = m_tanks[i]->get_row();
            int col = m_tanks[i]->get_col();

            if (dir == UP && row > 1) {
                mvaddch(row, col, '.');
                row -= 1;
                m_tanks[i]->set_row(row);
                mvaddch(row, col, 'T');
            }
            else if (dir == DOWN && row < g_screen->height() - 2) {
                mvaddch(row, col, '.');
                row += 1;
                m_tanks[i]->set_row(row);
                mvaddch(row, col, 'T');
            }
            else if (dir == LEFT && col > 2) {
                mvaddch(row, col, '.');
                col -= 1;
                m_tanks[i]->set_col(col);
                mvaddch(row, col, 'T');
            }
            else if (dir == RIGHT && col < g_screen->width() - 2) {
                mvaddch(row, col, '.');
                col += 1;
                m_tanks[i]->set_col(col);
                mvaddch(row, col, 'T');
            }
            refresh();
        }
        usleep(1000000);
    }
}

void Game::run() {
    // std::thread r_thread(&Game::robo_thread, this);
    // struct winsize w;
    // ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    // printw("\nHeight: %d Width: %d", w.ws_row, w.ws_col);
    std::thread p_thread(&Game::player_thread, this);
    std::thread r_thread(&Game::robo_thread, this);

    r_thread.join();
    p_thread.join();
}


int main(int, char *[]) {
    init_curses();
    printw("LETS GO");
    getch();
    clear();
    Screen screen = Screen();

    srand(time(NULL));
    int player_row = generate_random_pos(screen);
    int player_col = generate_random_pos(screen);
    Player player = Player(player_row, player_col);

    Game game = Game(&player, &screen, 10);
    game.run();

    endwin();
    return 0;
}

void init_curses() {
    initscr();
    clear();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);
}

int generate_random_pos(Screen screen) {
    int pos = rand() % (screen.height() - 3);
    return pos;
}
