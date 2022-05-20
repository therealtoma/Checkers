#include "player.hpp"

Player::Player(int player_nr) {
    std::cout << "constructor called" << std::endl;
    //checks if player number is valid otherwise throws an exception
    if (player_nr != 0 && player_nr != 1)
        throw player_exception{player_exception::index_out_of_bounds, "The player can only be 0 or 1"};

    this->board_nr = 1;
    this->player_nr = player_nr; // sets the player number

    memory = new Impl{nullptr}; // initializes the memory

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

    std::cout << "constructor ended" << std::endl;

} // constructor

// destructor
Player::~Player(){
    std::cout << "destructor called" << std::endl;
    // loops the list
    while(memory != nullptr){
        Impl* temp = memory; // saves the list address
        memory = memory->next; // goes ti the next node
        delete temp; // deletes the memory
    }
    delete memory; // deletes the last memory
    std::cout << "destructor ended" << std::endl;
}


// copy constructor
Player::Player(const Player& copy){
    std::cout << "copy constructor called" << std::endl;
    // sets the board number
    this->board_nr = copy.board_nr;
    // sets the player number
    this->player_nr = copy.player_nr;
    // allocates memory
    memory = new Impl{nullptr};
    pImpl start = this->memory; // saves the beginning of the list
    pImpl copyMemory = copy.memory; // saves a copy of the copy memory

    // loops the memory
    while(copyMemory){
        // saves the board
        for(int i = 0; i < 8; i++){
            for(int j = 0; j < 8; j++){
                this->memory->board[i][j] = copyMemory->board[i][j];
            }
        }
        memory = memory->next;
        if(copyMemory->next)
            memory = new Impl{nullptr};

        copyMemory = copyMemory->next;
    }
    // goes back to the beginning of the list
    this->memory = start;
    std::cout << "copy constructor ended" << std::endl;
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
/*
Player& operator=(const Player&){
    std::cout << "operator= called" << std::endl;
}
 */
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
void Player::store_board(const std::string& filename, int history_offset /* =0 */) const{
    std::cout << "store_board called" << std::endl;
    if(history_offset < 0 || history_offset > this->board_nr)
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
    try {
        Player p1(1);
        Player p2(1);

        Player p3(p1);

        p3.store_board("../Board1.txt", 0);
        std::cout << "fine" << std::endl;
    }
    catch(player_exception& e){
        std::cout << e.msg << std::endl;
    }
    return 0;
}