#include "player.hpp"
#define BOARD_SIZE 8

char convertToChar(Player::piece p){
    switch (p) {
        case Player::piece::x:
            return 'x';
        case Player::piece::X:
            return 'X';
        case Player::piece::o:
            return 'o';
        case Player::piece::O:
            return 'O';
        case Player::piece::e:
            return ' ';
    }
    return ' ';
}

Player::piece convertToPiece(char c){
    switch (c) {
        case 'x':
            return Player::piece::x;
        case 'X':
            return Player::piece::X;
        case 'o':
            return Player::piece::o;
        case 'O':
            return Player::piece::O;
        case ' ':
            return Player::piece::e;
    }
    return Player::piece::e;
}

void deleteBoard(Player::piece** (&board)){
    for(int i = 0; i < BOARD_SIZE; i++)
        delete[] board[i];
    delete[] board;
}

bool file_exists(const std::string& filename){
    std::ifstream f(filename.c_str());
    return f.good();
}

Player::piece** initialize_board(){
    Player::piece** matrix = nullptr;
    matrix = new Player::piece*[BOARD_SIZE];
    for(int i = 0; i < BOARD_SIZE; i++)
        matrix[i] = new Player::piece[BOARD_SIZE];

    return matrix;
}

struct Player::Impl{
    Impl* next;
    Player::piece** board; // the Dama board
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
        if(temp->board != nullptr) {
            deleteBoard(temp->board);
        }
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
    this->pimpl = new Impl{
    nullptr,
    initialize_board(),
    copy.pimpl->index,
        copy.pimpl->player_nr
    };

    for(int i = 0; i < BOARD_SIZE; i++) {
        for(int j = 0; j < BOARD_SIZE; j++) {
            this->pimpl->board[i][j] = copy.pimpl->board[i][j];
        }
    }

    Impl* copyTemp = copy.pimpl;
    Impl* thisTemp = this->pimpl;

    while(copyTemp->next) {
        thisTemp->next = new Impl{
            nullptr,
            initialize_board(),
            thisTemp->index + 1,
            copyTemp->player_nr
        };

        for(int i = 0; i < BOARD_SIZE; i++) {
            for(int j = 0; j < BOARD_SIZE; j++) {
                thisTemp->next->board[i][j] = copyTemp->next->board[i][j];
            }
        }
        thisTemp = thisTemp->next;
        copyTemp = copyTemp->next;
    }

    std::cout << "copy constructor ended" << std::endl;
} // copy constructor

// operator ()
/*Player::piece Player::operator()(int r, int c, int history_offset) const{
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

*/

// operator =
/*
Player& operator=(const Player&){
    std::cout << "operator= called" << std::endl;
}
 */


/**
 * Loads the board from the file and saves it in the most recent memory location
 * @param filename the file name
 */
void Player::load_board(const std::string& filename){
    std::cout << "load board called" << std::endl;

    if(this->pimpl->next == nullptr){
        this->pimpl->board = initialize_board();
    }

    Impl* temp = this->pimpl;
    int lastIndex = this->pimpl->index;
    // goes to the end of the player list
    while(temp->next) {
        lastIndex++;
        temp = temp->next;
    }

    if(!file_exists(filename))
        throw player_exception{player_exception::missing_file, "file not found"};

    std::fstream file(filename, std::fstream::in);
    Player::piece** board = initialize_board();
    char cella;
    int readCharacters = 0, i = BOARD_SIZE - 1, j = 0;

    while(file.get(cella)) {
        if(cella != '\n'){

            board[i][j] = convertToPiece(cella);
            std::cout << convertToChar(board[i][j]);
            j++;
            readCharacters++;
            if(j == BOARD_SIZE){
                j = 0;
                i--;
                std::cout << std::endl;
            }
        }
        file.get(cella);
    }
    file.close();

    if(readCharacters != BOARD_SIZE * BOARD_SIZE){

       deleteBoard(board);

       throw player_exception{player_exception::invalid_board, "board not valid"};
    }

    // adding the board to the last player memory
    temp->next = new Impl{
        nullptr,
        initialize_board(),
        lastIndex+1,
        this->pimpl->player_nr
    };
    temp = temp->next;

    for(i = 0; i < BOARD_SIZE; i++){
        for(j = 0; j < BOARD_SIZE; j++) {
            temp->board[i][j] = board[i][j];
        }
    }

    //deleteBoard(temp->board);
    deleteBoard(board);
    //delete temp1;
    std::cout << "load board ended" << std::endl;
}


// store board
/*
void Player::store_board(const std::string& filename, int history_offset) const{
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

/**
 * creates an initial board and stores it inside the first memory of the player and inside the file
 * @param filename the file to to store the board into
 */
void Player::init_board(const std::string& filename) const{
    // initial board
    std::cout << "init_board called" << std::endl;

    if(this->pimpl->next == nullptr){
        this->pimpl->board = initialize_board();
    }

    // allocates the memory
    Player::piece **initial_board = initialize_board();

    // fill starting board with the default field
    for(int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (i >= 0 && i <= 2)
                ((i + j) % 2) == 0 ? initial_board[i][j] = piece::e : initial_board[i][j] = piece::x;
            else if (i >= 5 && i <= 7)
                ((i + j) % 2) == 0 ? initial_board[i][j] = piece::e : initial_board[i][j] = piece::o;
            else
                initial_board[i][j] = piece::e;
        }
    }

    // goes to the end of the player memory
    Impl* temp = this->pimpl;
    int lastIndex = this->pimpl->index;
    // goes to the end of the player list
    while(temp->next) {
        lastIndex++;
        temp = temp->next;
    }

    temp->next = new Impl{
        nullptr,
        initialize_board(),
        lastIndex+1,
        this->pimpl->player_nr
    };
    temp = temp->next;

    // filling the board
    for(int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            temp->board[i][j] = initial_board[i][j];
        }
    }

    std::fstream file;
    file.open(filename, std::fstream::out);

    for(int i = BOARD_SIZE - 1; i >= 0; i--) {
        for(int j = 0; j < BOARD_SIZE; j++) {
            file << convertToChar(temp->board[i][j]);
            if(j != BOARD_SIZE - 1) file << ' ';
        }
        if(i != 0)
            file << "\n";
    }

    file.close();
    // deletes the temporary variables

    deleteBoard(initial_board);
    //deleteBoard(temp1->board);
    std::cout << "init_board ended" << std::endl;
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


        p1.init_board("./ciao.txt");
        p2.init_board("./ciao2.txt");
        //p2.init_board("./ciao.txt");
        //p1.load_board("./ciao.txt");
        //p2.load_board("./ciao.txt");
        //p1.load_board("./ciao2.txt");
        Player p3(p1);
    }
    catch(player_exception& e){
        std::cout << e.msg << std::endl;
    }
    return 0;
}