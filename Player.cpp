#include "player.hpp"

Player::Player(int player_nr) {

    //checks if player number is valid otherwise throws an exception
    if (player_nr != 0 && player_nr != 1)
        throw player_exception{player_exception::index_out_of_bounds, "The player can only be 0 or 1"};

    this->board_nr = 1;
    this->player_nr = player_nr; // sets the player number

    this->memory = new Impl; // creates a new memory
    this->memory->next = nullptr;
    this->memory->prev = nullptr;

    /* initializes the starting board (in the stack)*/
    // first row
    memory->board[0][0] = e;
    memory->board[0][1] = x;
    memory->board[0][2] = e;
    memory->board[0][3] = e;
    memory->board[0][4] = e;
    memory->board[0][5] = o;
    memory->board[0][6] = e;
    memory->board[0][7] = o;

    // second row
    memory->board[1][0] = x;
    memory->board[1][1] = e;
    memory->board[1][2] = x;
    memory->board[1][3] = e;
    memory->board[1][4] = e;
    memory->board[1][5] = e;
    memory->board[1][6] = o;
    memory->board[1][7] = e;

    //third row
    memory->board[2][0] = e;
    memory->board[2][1] = x;
    memory->board[2][2] = e;
    memory->board[2][3] = e;
    memory->board[2][4] = e;
    memory->board[2][5] = o;
    memory->board[2][6] = e;
    memory->board[2][7] = o;

    //fourth row
    memory->board[3][0] = x;
    memory->board[3][1] = e;
    memory->board[3][2] = x;
    memory->board[3][3] = e;
    memory->board[3][4] = e;
    memory->board[3][5] = e;
    memory->board[3][6] = o;
    memory->board[3][7] = e;

    //fifth row
    memory->board[4][0] = e;
    memory->board[4][1] = x;
    memory->board[4][2] = e;
    memory->board[4][3] = e;
    memory->board[4][4] = e;
    memory->board[4][5] = o;
    memory->board[4][6] = e;
    memory->board[4][7] = o;

    //sixth row
    memory->board[5][0] = x;
    memory->board[5][1] = e;
    memory->board[5][2] = x;
    memory->board[5][3] = e;
    memory->board[5][4] = e;
    memory->board[5][5] = e;
    memory->board[5][6] = o;
    memory->board[5][7] = e;

    //seventh row
    memory->board[6][0] = e;
    memory->board[6][1] = x;
    memory->board[6][2] = e;
    memory->board[6][3] = e;
    memory->board[6][4] = e;
    memory->board[6][5] = o;
    memory->board[6][6] = e;
    memory->board[6][7] = o;

    //eighth row
    memory->board[7][0] = x;
    memory->board[7][1] = e;
    memory->board[7][2] = x;
    memory->board[7][3] = e;
    memory->board[7][4] = e;
    memory->board[7][5] = e;
    memory->board[7][6] = o;
    memory->board[7][7] = e;

    std::cout << "costruttore chiamato" << std::endl;
} // constructor

// destructor
Player::~Player(){
    while(this->memory){
        Impl* temp = memory;
        memory = memory->next;
        delete temp;
    }
    std::cout << "distruttore chiamato" << std::endl;
}

// copy constructor
Player::Player(const Player& copy){

    this->board_nr = copy.board_nr;
    this->player_nr = copy.player_nr;

    // torno all'inizio delle liste
    Impl* copyMemory = copy.memory;
    while(copyMemory->prev != nullptr)
        copyMemory = copyMemory->prev;

    while(this->memory->prev != nullptr)
        this->memory = this->memory->prev;

    Impl* begin = this->memory;
    while(copyMemory != nullptr){
        if(this->memory->next == nullptr){
            this->memory->next = new Impl;
            this->memory->next->next = nullptr;
            this->memory->next->prev = this->memory;
        }

        for(int i = 0; i < 8; i++){
            for(int j = 0; j < 8; j++){
                this->memory->board[i][j] = copyMemory->board[i][j];
            }
        }

        copyMemory = copyMemory->next;
        this->memory = this->memory->next;
    }

    this->memory = begin;

    std::cout << "copy constructor called" << std::endl;
}

Player &Player::operator=(const Player &copy) {
    return *this;
}

Player::piece Player::operator()(int r, int c, int history_offset /* =0 */) const{
    if(r < 0 || r > 7 || c < 0 || c > 7)
        throw player_exception{player_exception::index_out_of_bounds, "coordinates not valid"};

    if(history_offset < 0 || history_offset > this->board_nr)
        throw player_exception{player_exception::index_out_of_bounds, "the board number is not valid"};

    // sets a temporary board to use in the loop
    pImpl tempBoard = this->memory;
    // loops the boards
    while(history_offset >= 0){
        if(history_offset == 0)
            return this->memory->board[r][c]; // returns the history_offsetTH piece in position r, c
        tempBoard = tempBoard->next;
        history_offset--;
    }
    std::cout << "operator() called " << std::endl;
    return e; // used as escape value

}

void Player::load_board(const std::string& filename){
    std::cout << "load board called" << std::endl;

}
void Player::store_board(const std::string& filename, int history_offset /* =0 */) const{
    std::cout << "store_board called" << std::endl;
}
void Player::init_board(const std::string& filename) const{
    std::cout << "init_board called" << std::endl;
}
void Player::move(){
    std::cout << "move called" << std::endl;
}
bool Player::valid_move() const{
    std::cout << "valid_move called" << std::endl;
    return true;
}
void Player::pop(){
    std::cout << "pop called" << std::endl;
}
bool Player::wins(int player_nr) const{
    std::cout << "wins called" << std::endl;
    return true;
}
bool Player::wins() const{
    std::cout << "wins called" << std::endl;
    return true;
}
bool Player::loses(int player_nr) const{
    std::cout << "loses called" << std::endl;
    return true;
}
bool Player::loses() const{
    std::cout << "loses called" << std::endl;
    return true;
}
int Player::recurrence() const{
    std::cout << "recurrence called" << std::endl;
    return 0;
}


int main(){

    Player p1(1);

    Player p2 = p1;

    return 0;

    return 0;
}