#include <string>
#include <iostream>

const int MAXTANKS = 100;
const int MAXROWS = 20;
const int MAXCOLS = 20;
const int MAXSHOTLEN = 5;

enum {
    UP,
    DOWN,
    LEFT,
    RIGHT
};



class Player {
};

class Tank {
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
