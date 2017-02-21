#include <stdio.h>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <ncurses.h>

#include "globals.h"
#include "Player.h"
#include "Screen.h"
#include "Tank.h"
#include "Game.h"

void init_curses();
void game_loop(int, Screen);
int generate_random_pos(Screen);

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
