#include "Player.h"

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

char Player::get_shooting_dir() const {
    return m_shooting_dir;
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

void Player::set_pos(int row, int col) {
    m_row = row;
    m_col = col;
}

void Player::set_shooting_dir(char dir) {
    m_shooting_dir = dir;
}
