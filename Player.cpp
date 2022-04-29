#include "player.hpp"

enum piece {x, o, X, O, e};

Player::Player(int player_nr) {

    //checks if player number is valid otherwise throws an exception
    if (player_nr != 0 && player_nr != 1)
        throw player_exception{player_exception::index_out_of_bounds, "The player can only be 0 or 1"};

    this->player_nr = player_nr; // sets the player number

    memory = new Impl; // initializes the memory
    memory->next = nullptr; // on the first call, the next pointer is nullptr
    memory->prev = nullptr; // on the first call, the prev pointer is nullptr

    /* creates the starting board */
    // first row
    memory->board[0][0] = e;
    memory->board[0][1] = x;
    memory->board[0][2] = e;
    memory->board[0][3] = x;
    memory->board[0][4] = e;
    memory->board[0][5] = x;
    memory->board[0][6] = e;
    memory->board[0][7] = x;

    // second row
    memory->board[1][0] = x;
    memory->board[1][1] = e;
    memory->board[1][2] = x;
    memory->board[1][3] = e;
    memory->board[1][4] = x;
    memory->board[1][5] = e;
    memory->board[1][6] = x;
    memory->board[1][7] = e;

    //third row
    memory->board[2][0] = e;
    memory->board[2][1] = x;
    memory->board[2][2] = e;
    memory->board[2][3] = x;
    memory->board[2][4] = e;
    memory->board[2][5] = x;
    memory->board[2][6] = e;
    memory->board[2][7] = x;

    //fourth row
    memory->board[3][0] = x;
    memory->board[3][1] = e;
    memory->board[3][2] = x;
    memory->board[3][3] = e;
    memory->board[3][4] = x;
    memory->board[3][5] = e;
    memory->board[3][6] = x;
    memory->board[3][7] = e;

    //fifth row
    memory->board[4][0] = e;
    memory->board[4][1] = x;
    memory->board[4][2] = e;
    memory->board[4][3] = x;
    memory->board[4][4] = e;
    memory->board[4][5] = x;
    memory->board[4][6] = e;
    memory->board[4][7] = x;

    //sixth row
    memory->board[5][0] = x;
    memory->board[5][1] = e;
    memory->board[5][2] = x;
    memory->board[5][3] = e;
    memory->board[5][4] = x;
    memory->board[5][5] = e;
    memory->board[5][6] = x;
    memory->board[5][7] = e;

    //seventh row
    memory->board[6][0] = e;
    memory->board[6][1] = x;
    memory->board[6][2] = e;
    memory->board[6][3] = x;
    memory->board[6][4] = e;
    memory->board[6][5] = x;
    memory->board[6][6] = e;
    memory->board[6][7] = x;

    //eighth row
    memory->board[7][0] = x;
    memory->board[7][1] = e;
    memory->board[7][2] = x;
    memory->board[7][3] = e;
    memory->board[7][4] = x;
    memory->board[7][5] = e;
    memory->board[7][6] = x;
    memory->board[7][7] = e;

    std::cout << "Player " << player_nr << " created!\n";
} // constructor

/*
Player::~Player(){
}
Player::Player(const Player&){
}
Player::piece Player::operator()(int r, int c, int history_offset = 0) const{
}
void Player::load_board(const std::string& filename){}
void Player::store_board(const std::string& filename, int history_offset = 0) const{}
void Player::init_board(const std::string& filename) const{}
void Player::move(){}
bool Player::valid_move() const{}
void Player::pop(){}
bool Player::wins(int player_nr) const{}
bool Player::wins() const{}
bool Player::loses(int player_nr) const{}
bool Player::loses() const{}
int Player::recurrence() const{}
*/
int player_nr;

struct Impl{
    Impl* next;
    Impl* prev;

    Player::piece board[8][8];
};

typedef Impl* pImpl;

pImpl memory; // a pointer to the latest board
