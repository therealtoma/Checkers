#include "player.hpp"
#define BOARD_SIZE 8

char convert_to_char(Player::piece p){
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

Player::piece convert_to_piece(char c){
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

void delete_board(Player::piece** (&board)){
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
    if (player_nr != 1 && player_nr != 2)
        throw player_exception{player_exception::index_out_of_bounds, "The player number can only be 1 or 2"};

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
            delete_board(temp->board);
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

    Impl* copy_temp = copy.pimpl;
    Impl* this_temp = this->pimpl;

    while(copy_temp->next) {
        this_temp->next = new Impl{
            nullptr,
            initialize_board(),
            this_temp->index + 1,
            copy_temp->player_nr
        };

        for(int i = 0; i < BOARD_SIZE; i++) {
            for(int j = 0; j < BOARD_SIZE; j++) {
                this_temp->next->board[i][j] = copy_temp->next->board[i][j];
            }
        }
        this_temp = this_temp->next;
        copy_temp = copy_temp->next;
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
Player& Player::operator=(const Player& p){
    std::cout << "operator= called" << std::endl;
    while(this->pimpl){
        Impl* temp = this->pimpl;
        this->pimpl = this->pimpl->next;
        if(temp->board != nullptr) {
            delete_board(temp->board);
        }
        delete temp;
    }
    delete pimpl;
    this->pimpl = new Impl{
            nullptr,
            initialize_board(),
            p.pimpl->index,
            p.pimpl->player_nr
    };

    for(int i = 0; i < BOARD_SIZE; i++) {
        for(int j = 0; j < BOARD_SIZE; j++) {
            this->pimpl->board[i][j] = p.pimpl->board[i][j];
        }
    }

    Impl* copy_temp = p.pimpl;
    Impl* this_temp = this->pimpl;

    while(copy_temp->next) {
        this_temp->next = new Impl{
                nullptr,
                initialize_board(),
                this_temp->index + 1,
                copy_temp->player_nr
        };

        for(int i = 0; i < BOARD_SIZE; i++) {
            for(int j = 0; j < BOARD_SIZE; j++) {
                this_temp->next->board[i][j] = copy_temp->next->board[i][j];
            }
        }
        this_temp = this_temp->next;
        copy_temp = copy_temp->next;
    }
    std::cout << "operator= ended" << std::endl;
    return *this;
}


/**
 * Loads the board from the file and saves it in the most recent memory location
 * @param filename the file name
 */
void Player::load_board(const std::string& filename){
    std::cout << "load board called" << std::endl;

    Impl* temp = this->pimpl;
    int last_index = this->pimpl->index;

    if(this->pimpl->board == nullptr){
        this->pimpl->board = initialize_board();
        temp = this->pimpl;
    }
    else{
        // goes to the end of the player list
        while(temp->next) {
            last_index++;
            temp = temp->next;
        }
        temp->next = new Impl{
                nullptr,
                initialize_board(),
                lat_index + 1,
                this->pimpl->player_nr
        };
        temp = temp->next;
    }

    if(!file_exists(filename))
        throw player_exception{player_exception::missing_file, "file not found"};

    std::fstream file(filename, std::fstream::in);
    Player::piece** board = initialize_board();
    char cella;
    int read_characters = 0, i = BOARD_SIZE - 1, j = 0;

    while(file.get(cella)) {
        if(cella != '\n'){

            board[i][j] = convert_to_piece(cella);
            std::cout << convert_to_char(board[i][j]);
            j++;
            read_characters++;
            if(j == BOARD_SIZE){
                j = 0;
                i--;
                std::cout << std::endl;
            }
        }
        file.get(cella);
    }
    file.close();

    if(read_characters!= BOARD_SIZE * BOARD_SIZE){

       delete_board(board);

       throw player_exception{player_exception::invalid_board, "board not valid"};
    }

    for(i = 0; i < BOARD_SIZE; i++){
        for(j = 0; j < BOARD_SIZE; j++) {
            temp->board[i][j] = board[i][j];
        }
    }
    delete_board(board);

    std::cout << "load board ended" << std::endl;
}

/**
 * Saves to a file the histroy_offset'th board ( 0 = most recent and so on )
 * @param filename the name of the file
 * @param history_offset the number of the board
 */
void Player::store_board(const std::string& filename, int history_offset) const{
    std::cout << "store_board called" << std::endl;

    Impl* temp = this->pimpl;
    int memory_size = 0;

    while(temp->next){
        temp = temp->next;
        memory_size++;
    }

    if(history_offset >= memory_size)
        throw player_exception{player_exception::index_out_of_bounds, "The inserted history_offset is not valid"};

    int index = memory_size - 1;
    temp = this->pimpl;

    while(index != history_offset){
        temp = temp->next;
        index--;
    }

    std::fstream file;
    file.open(filename, std::fstream::out);

    for(int i = BOARD_SIZE - 1; i >= 0; i--) {
        for(int j = 0; j < BOARD_SIZE; j++) {
            file << convert_to_char(temp->board[i][j]);
            if(j != BOARD_SIZE - 1) file << ' ';
        }
        if(i != 0)
            file << "\n";
    }

    file.close();

    std::cout << "store_board ended" << std::endl;
}

/**
 * creates an initial board and stores it inside the first memory of the player and inside the file
 * @param filename the file to to store the board into
 */
void Player::init_board(const std::string& filename) const{
    // initial board
    std::cout << "init_board called" << std::endl;

    Impl* temp = this->pimpl;
    int last_index = this->pimpl->index;

    if(this->pimpl->board == nullptr){
        this->pimpl->board = initialize_board();
        temp = this->pimpl;
    }
    else{
        // goes to the end of the player list
        while(temp->next) {
            last_index++;
            temp = temp->next;
        }
        temp->next = new Impl{
                nullptr,
                initialize_board(),
                last_index + 1,
                this->pimpl->player_nr
        };
        temp = temp->next;
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
            file << convert_to_char(temp->board[i][j]);
            if(j != BOARD_SIZE - 1) file << ' ';
        }
        if(i != 0)
            file << "\n";
    }

    file.close();
    // deletes the temporary variables

    delete_board(initial_board);
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
        Player p2(2);


        p1.load_board("./ciao.txt");
        p1.load_board("./ciao2.txt");
        /*
        p1.init_board("./ciao2.txt");
        p2.init_board("./ciao.txt");
        p1.load_board("./ciao.txt");
        p2.load_board("./ciao.txt");
        p1.load_board("./ciao2.txt");
        p1.store_board("test1.txt", 0);
        */
        p2 = p1;
        p2.store_board("./test1.txt", 0);
    }
    catch(player_exception& e){
        std::cout << e.msg << std::endl;
    }
    return 0;
}