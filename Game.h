#pragma once
#include "Screen.h"
#include "Player.h"
#include "Tank.h"
#include <vector>
#include <thread>
//#include <mutex>

class Game {
    public:
        Game(Player *p, Screen *s, int num_tanks);
        void init_robo_tanks();
        void robo_thread();
        void player_thread();
        void run();
        void join_thread(std::thread &t);
        void shooting_thread();
        void test();
        int generate_random_pos();
    private:
        Player  *g_player;
        Screen  *g_screen;
        bool     g_running;
        int      m_num_tanks;
        Tank    *m_tanks[MAXTANKS];
        std::vector<std::thread> g_threads;

};
