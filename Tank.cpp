#include "Tank.h"

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
