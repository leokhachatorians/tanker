#include "Game.h"
#include <ncurses.h>
#include <stdlib.h>
#include <thread>
#include <unistd.h>

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
            g_player->set_shooting_dir('l');
            g_player->set_pos(row, col);
        }
        else if (ch == KEY_RIGHT && col < g_screen->width() - 2) {
            mvaddch(row, col, '.');
            col += 1;
            mvaddch(row, col, g_player->get_sym());
            refresh();
            g_player->set_shooting_dir('r');
            g_player->set_pos(row, col);
        }
        else if (ch == KEY_UP && row > 1) {
            mvaddch(row, col, '.');
            row -= 1;
            mvaddch(row, col, g_player->get_sym());
            refresh();
            g_player->set_shooting_dir('u');
            g_player->set_pos(row, col);
        }
        else if (ch == KEY_DOWN && row < g_screen->height() - 3) {
            mvaddch(row, col, '.');
            row += 1;
            mvaddch(row, col, g_player->get_sym());
            refresh();
            g_player->set_shooting_dir('d');
            g_player->set_pos(row, col);
        }
        else if (ch == ' ') {
            char shooting_dir = g_player->get_shooting_dir();
            int row = g_player->get_row();
            int col = g_player->get_col();
            switch (shooting_dir) {
                case 'l':
                    for (int i = 0; i < 10; i++) {
                        mvaddch(row, col-i,'<');
                        refresh();
                    }
                    break;
                case 'r':
                    for (int i = 0; i < 10; i++) {
                        mvaddch(row, col+i, '>');
                        refresh();
                    }
                    break;
                case 'u':
                    for (int i = 0; i < 10; i++) {
                        mvaddch(row-i, col, '^');
                        refresh();
                    }
                    break;
                case 'd':
                    for (int i = 0; i < 10; i++) {
                        mvaddch(row+i, col, 'v');
                        refresh();
                    }
                    break;
                default: break;
            }
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
        usleep(500000);
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
