#include "player.hpp"
#define BOARD_SIZE 8 * 8

struct Player::Impl{
    Impl* next;
    Player::piece* board; // the Dama board
    int index; // the index of the board
    int player_nr; // the player number
};

/**
 * @brief Construct a new Player object
 * @param player_nr the player number
 */
Player::Player(int player_nr) {

    std::cout << "constructor called" << std::endl;

    //checks if player number is valid otherwise throws an exception
    if (player_nr != 0 && player_nr != 1)
        throw player_exception{player_exception::index_out_of_bounds, "The player can only be 0 or 1"};

    pimpl = new Impl{nullptr, nullptr, 0, player_nr}; // initializes the memory

    std::cout << "constructor ended" << std::endl;

} // constructor

/**
 * @brief Destroy the Player object
 */
Player::~Player(){
    std::cout << "destructor called" << std::endl;
    // loops the list
    while(pimpl != nullptr){
        Impl* temp = pimpl; // saves the list address
        pimpl = pimpl->next; // goes to the next node
        delete[] temp->board;
        delete temp; // deletes the memory
    }
    delete pimpl; // deletes the last memory
    std::cout << "destructor ended" << std::endl;
} // destructor

/**
 * @brief Copy constructor
 * @param copy the other player
 */
Player::Player(const Player& copy){
    std::cout << "copy constructor called" << std::endl;

    // allocates memory
    this->pimpl = new Impl{ nullptr };

    Impl* thisMemory = this->pimpl; // saves the beginning of the list
    Impl* copyMemory = copy.pimpl; // saves a copy of the copy memory

    // loops the copy
    while(copyMemory){
        // saves the board
        thisMemory->index = copyMemory->index;
        thisMemory->player_nr = copyMemory->player_nr;
        thisMemory->board = new piece[BOARD_SIZE];
        for(int i = 0; i < BOARD_SIZE/2; i++){
            for(int j = 0; j < BOARD_SIZE/2; j++){
                thisMemory->board[i * BOARD_SIZE/2 + j] = copyMemory->board[i * BOARD_SIZE/2 + j];
            }
        }
        thisMemory = thisMemory->next;

        if(copyMemory->next)
            thisMemory = new Impl{nullptr};

        copyMemory = copyMemory->next;
    }

    std::cout << "copy constructor ended" << std::endl;
} // copy constructor

// operator ()
/*Player::piece Player::operator()(int r, int c, int history_offset *//* =0 *//*) const{
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

*//*
Player& operator=(const Player&){
    std::cout << "operator= called" << std::endl;
}
 *//*

// load board
void Player::load_board(const std::string& filename){
    std::cout << "load board called" << std::endl;

    pImpl it = this->memory;

    while(it->next)
        it = it->next;

    it->next = new Impl{nullptr};
    it = it->next;
// sostituire il for con il sistema per leggere da file
    std::cout << "file: " << filename << std::endl;
    std::fstream file(filename);
    if(!file) throw player_exception{player_exception::missing_file, "There was a problem with the files, ending!"};
    // loop till the end of the file
    std::string line;
    int i, j = 7;
    while(std::getline(file, line)){
        // check the character and store it in the enum board
        if(line.length() != 8)
            throw player_exception{player_exception::invalid_board, "the board is not valid"};
        i = 0;
        for(char c : line) {
            switch (c) {
                case ' ':
                    it->board[i][j] = e;
                    break;
                case 'x':
                    it->board[i][j] = x;
                    break;
                case 'X':
                    it->board[i][j] = X;
                    break;
                case 'o':
                    it->board[i][j] = o;
                    break;
                case 'O':
                    it->board[i][j] = O;
                    break;
                default:
                    throw player_exception{player_exception::missing_file,
                                           "There was a problem with the structure of the file, ending"};
            }
            i++;
        }
        j--;
    }
    file.close(); // closing the file
    board_nr++; // incrementing the board number
    std::cout << "load board terminated" << std::endl;

}

// store board
void Player::store_board(const std::string& filename, int history_offset *//* =0 *//*) const{
    std::cout << "store_board called" << std::endl;
    if(history_offset < 0)
        throw player_exception{player_exception::index_out_of_bounds, "the board number is not valid"};




    pImpl it = this->memory;
    int i = this->board_nr - 1;

    while(i != history_offset){
        it = it->next;
        i--;
    }

    pImpl nextNode = it->next;
    it->next = new Impl{nextNode};
    std::fstream file(filename);
    std::string line;
    int j = 7;
    while(std::getline(file, line)){
        // check the character and store it in the enum board
        if(line.length() != 15) // the 8 cells plus the spaces between them
            throw player_exception{player_exception::invalid_board, "the board is not valid"};
        i = 0;
        // loops the line skipping the spaces between the cells
        for(int k = 0; k < 15; k+=2) {
            char c = line.at(k);
            switch (c) {
                case ' ':
                    it->board[i][j] = e;
                    break;
                case 'x':
                    it->board[i][j] = x;
                    break;
                case 'X':
                    it->board[i][j] = X;
                    break;
                case 'o':
                    it->board[i][j] = o;
                    break;
                case 'O':
                    it->board[i][j] = O;
                    break;
                default:
                    throw player_exception{player_exception::missing_file, "There was a problem with the structure of the file, ending"};
            }
            i++;
        }
        j--;
    }

    file.close();

    std::cout << "store_board ended" << std::endl;
}*/

// init board
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
    try {
        Player p1(0);
        Player p2(1);

        //Player p3(p1);

        // p3.store_board("../Board1.txt", 0);
    }
    catch(player_exception& e){
        std::cout << e.msg << std::endl;
    }
    return 0;
}