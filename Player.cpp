#include "player.hpp"
#define BOARD_SIZE 8
#define NUMBER_OF_x 12
#define NUMBER_OF_o 12
/**
 * converts a piece into char
 * @param p the piece to convert
 * @return the converted character '0' in case of error
 */
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
    throw player_exception{player_exception::invalid_board, "The insered piece is not valid"};
}

/**
 * converts a piece into a string
 * @param c the character to convert
 * @return tha converted piece, e for default
 */
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
        default:
            break;
    }
    throw player_exception{player_exception::invalid_board, "The inserted haracter is not valid"};
}

/**
 * allocates the memory for a board
 * @return the allocated board
 */
Player::piece** initialize_board(){
    Player::piece** matrix = nullptr;
    matrix = new Player::piece*[BOARD_SIZE];
    for(int i = 0; i < BOARD_SIZE; i++)
        matrix[i] = new Player::piece[BOARD_SIZE];

    return matrix;
}

/**
 * deallocates the memory for the passed board
 * @param board the boad to deallocate
 */
void delete_board(Player::piece** (&board)){
    for(int i = 0; i < BOARD_SIZE; i++)
        delete[] board[i];
    delete[] board;
}

/**
 * checks whether a file exists
 * @param filename the name of the file to check
 * @return true if the file exists, false otherwise
 */
bool file_exists(const std::string& filename){
    std::ifstream f(filename.c_str());
    return f.good();
}
// struct Move code
struct Move{
    std::pair<int, int> current_position;
    std::pair<int, int>* available_moves;
    std::pair<std::pair<int, int>, int>* evaluations;
    Player::piece piece;

    std::pair<int, int>* get_available_pieces(int player_nr, Player::piece** board, int &arr_size){
        Player::piece piece_to_find = (player_nr == 1) ? Player::piece::x : Player::piece::o;
        Player::piece dame_to_find = (player_nr == 1) ? Player::piece::X : Player::piece::O;
        for(int i = 0; i < BOARD_SIZE; i++){
            for(int j = 0; j < BOARD_SIZE; j++) {
                if(board[i][j] == piece_to_find || board[i][j] == dame_to_find)
                    arr_size++;
            }
        }

        auto valid_positions = new std::pair<int, int>[arr_size];
        for(int i = 0; i < BOARD_SIZE; i++){
            for(int j = 0; j < BOARD_SIZE; j++) {
                if(board[i][j] == piece_to_find || board[i][j] == dame_to_find)
                    valid_positions[i] = std::make_pair(i, j);
            }
        }
        return valid_positions;
    }

    void append(std::pair<int, int> element, std::pair<int, int>* &array, int &array_size){
        array_size++;
        auto new_array = new std::pair<int, int>[array_size];
        for(int i = 0; i < array_size - 1 ; i++)
            new_array[i] = array[i];
        new_array[array_size - 1] = element;
        delete[] array;
        array = new_array;
    }

    // find available moves
    std::pair<int, int>* get_available_moves(std::pair<int, int> position, Player::piece** board) {
        int available_moves_number = 0;
        auto available_moves = new std::pair<int, int>[available_moves_number];
        Player::piece piece = board[position.first][position.second];

        if(piece == Player::piece::X || piece == Player::piece::O) {
            if(position.first == 0) {
                switch (position.second) {
                    case 0:
                        // check if it can go top-right
                        break;
                    case BOARD_SIZE - 1:
                        //  chech if it can go bottom-right
                        break;
                    default:
                        //it can go either top-right or bottom-right
                        break;
                }
            }
            else if ( position.first == BOARD_SIZE - 1) {
                switch (position.second){
                    case 0:
                        // can go top-left
                        break;
                    case BOARD_SIZE - 1:
                        // can go bottom-left
                        break;
                    default:
                        // can go either top-left or bottom-left
                        break;
                }
            }
            else {
                switch (position.second) {
                    case 0:
                        // can go top-left or top-right
                        break;
                    case BOARD_SIZE - 1:
                        // can go bottom-left or bottom-right
                        break;
                    default:
                        // can go any way
                        break;
                }
            }
        }
        return available_moves;
    }

    /*std::pair<std::pair<int, int>, int>* get_evaluations(int player_nr, Player::piece** board, int &arr_size){
        Player::piece piece_to_find = (player_nr == 1) ? Player::piece::x : Player::piece::o;
        Player::piece dame_to_find = (player_nr == 1) ? Player::piece::X : Player::piece::O;
        for(int i = 0; i < BOARD_SIZE; i++){
            for(int j = 0; j < BOARD_SIZE; j++) {
                if(board[i][j] == piece_to_find || board[i][j] == dame_to_find)
                    arr_size++;
            }
        }

        auto valid_positions = new std::pair<std::pair<int, int>, int>[arr_size];
        for(int i = 0; i < BOARD_SIZE; i++){
            for(int j = 0; j < BOARD_SIZE; j++) {
                if(board[i][j] == piece_to_find || board[i][j] == dame_to_find)
                    valid_positions[i] = std::make_pair(std::make_pair(i, j), 0);
            }
        }
        return valid_positions;
    }*/

};
// end struct Move code
struct Player::Impl{
    Impl* next;
    Player::piece** board; // the dama board
    int index; // the index of the board
    int player_nr; // the player number
};

/**
 * Construct a new Player object
 * @param player_nr the player number
 */
Player::Player(int player_nr) {
    std::cout << "constructor called" << std::endl;

    //checks if player number is valid otherwise throws an exception
    if (player_nr != 1 && player_nr != 2)
        throw player_exception{player_exception::index_out_of_bounds, "The player number can only be 1 or 2"};

    pimpl = new Impl{nullptr, nullptr, 0, player_nr}; // initializes the memory


} // constructor

/**
 * Destroy the Player object
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
} // destructor

/**
 * Copy constructor
 * @param copy the other player
 */
Player::Player(const Player& copy){
    std::cout << "copy constructor called" << std::endl;
    if(copy.pimpl->board != nullptr) {
        this->pimpl = new Impl{
                nullptr,
                initialize_board(),
                copy.pimpl->index,
                copy.pimpl->player_nr
        };

        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                this->pimpl->board[i][j] = copy.pimpl->board[i][j];
            }
        }

        Impl *copy_temp = copy.pimpl;
        Impl *this_temp = this->pimpl;

        while (copy_temp->next) {
            this_temp->next = new Impl{
                    nullptr,
                    initialize_board(),
                    this_temp->index + 1,
                    copy_temp->player_nr
            };

            for (int i = 0; i < BOARD_SIZE; i++) {
                for (int j = 0; j < BOARD_SIZE; j++) {
                    this_temp->next->board[i][j] = copy_temp->next->board[i][j];
                }
            }
            this_temp = this_temp->next;
            copy_temp = copy_temp->next;
        }
    }
    else
        this->pimpl = new Impl{nullptr, nullptr, copy.pimpl->index, copy.pimpl->player_nr};

} // copy constructor

/**
 * gets the piece in position r, c in the history_offset's board, throws an exception if r, c or history_offset are not valid
 * @param r the row of the piece to return
 * @param c the column of the piece to return
 * @param history_offset the board number ( 0 is the latest board inserted )
 * @return the corresponding piece
 */
Player::piece Player::operator()(int r, int c, int history_offset) const{

    // checkin if the row is within the range 0 <= r < BOARD_SIZE
    if(r >= BOARD_SIZE || r < 0)
        throw player_exception{player_exception::index_out_of_bounds, "The inserted row is not valid"};

    // checkin if the row is within the range 0 <= r < BOARD_SIZE
    if(c >= BOARD_SIZE || c < 0)
        throw player_exception{player_exception::index_out_of_bounds, "The inserted column is not valid"};

    Impl* temp = this->pimpl;
    int memory_size = 0;
    (this->pimpl->board != nullptr) ? memory_size = 1 : throw player_exception{player_exception::index_out_of_bounds, "The inserted history_offset is not valid"};
    // calculating memory size
    while(temp->next){
        temp = temp->next;
        memory_size++;
    }

    // checking history_offset validity
    if(history_offset >= memory_size)
        throw player_exception{player_exception::index_out_of_bounds, "The inserted history_offset is not valid"};

    // calculating the index of the chosen board
    int index = memory_size - 1;
    temp = this->pimpl;

    // going to the chosen board
    while(index != history_offset){
        temp = temp->next;
        index--;
    }

    return temp->board[r][c];

}

/**
 * puts the player p inside the caller player
 * @param p the player to copy
 * @return a reference to the updated player
 */
Player& Player::operator=(const Player& p){
    std::cout << "operator= called" << std::endl;
    // avoiding self assignment
    if(&p != this) {
        while (this->pimpl) {
            Impl *temp = this->pimpl;
            this->pimpl = this->pimpl->next;
            if (temp->board != nullptr)
                delete_board(temp->board);
            delete temp;
        }
        delete pimpl;
        this->pimpl = new Impl{
                nullptr,
                initialize_board(),
                p.pimpl->index,
                p.pimpl->player_nr
        };

        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                this->pimpl->board[i][j] = p.pimpl->board[i][j];
            }
        }

        Impl *copy_temp = p.pimpl;
        Impl *this_temp = this->pimpl;

        while (copy_temp->next) {
            this_temp->next = new Impl{
                    nullptr,
                    initialize_board(),
                    this_temp->index + 1,
                    copy_temp->player_nr
            };

            for (int i = 0; i < BOARD_SIZE; i++) {
                for (int j = 0; j < BOARD_SIZE; j++) {
                    this_temp->next->board[i][j] = copy_temp->next->board[i][j];
                }
            }
            this_temp = this_temp->next;
            copy_temp = copy_temp->next;
        }
    }
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
                last_index + 1,
                this->pimpl->player_nr
        };
        temp = temp->next;
    }

    if(!file_exists(filename))
        throw player_exception{player_exception::missing_file, "file not found"};

    std::fstream file(filename, std::fstream::in);
    Player::piece** board = initialize_board();
    char cella;
    int read_characters = 0, i = BOARD_SIZE - 1, j = 0, count_x = 0, count_o = 0;

    while(file.get(cella)) {
        if(cella != '\n'){
            // the file containes too many characters
            if(i < 0) {
                delete_board(board);
                throw player_exception{player_exception::invalid_board,
                                       "the board containes more values than it should."};
            }
            // a piece is in the white space
            if((i + j) % 2 == 0 && cella != ' ') {
                delete_board(board);
                throw player_exception{player_exception::invalid_board,
                                       "there's a piece in a not allowed space"};
            }
            // counts the number of o's and x's
            if(cella == 'o' || cella == 'O') count_o++;
            if(cella == 'x' || cella == 'X') count_x++;

            board[i][j] = convert_to_piece(cella);
            j++;
            read_characters++;
            if(j == BOARD_SIZE){
                j = 0;
                i--;
            }
        }
        file.get(cella);
    }
    file.close();

    if(read_characters != BOARD_SIZE * BOARD_SIZE){
       delete_board(board);
       throw player_exception{player_exception::invalid_board, "EXCEPTION: the selected board (" + filename+ ") is not valid. The problem is about it's size."};
    }

    if(count_x > NUMBER_OF_x){
        delete_board(board);
        throw player_exception{player_exception::invalid_board, "there are too many x pieces in the board"};
    }

    if(count_o > NUMBER_OF_o){
        delete_board(board);
        throw player_exception{player_exception::invalid_board, "there are too many o pieces in the board"};
    }

    for(i = 0; i < BOARD_SIZE; i++){
        for(j = 0; j < BOARD_SIZE; j++) {
            temp->board[i][j] = board[i][j];
        }
    }
    delete_board(board);
}

/**
 * Saves to a file the history_offset'th board ( 0 = most recent and so on )
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

}

/**
 * creates an initial board and stores it inside the first memory of the player and inside the file
 * @param filename the file to to store the board into
 */
void Player::init_board(const std::string& filename) const{
    // initial board
    std::cout << "init_board called" << std::endl;

    // allocates the memory
    Player::piece **initial_board = initialize_board();

    // fill starting board with the default field
    for(int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (i >= 0 && i <= 2)
                ((i + j) % 2) == 0 ? initial_board[i][j] = Player::piece::e : initial_board[i][j] = Player::piece::x;
            else if (i >= 5 && i <= 7)
                ((i + j) % 2) == 0 ? initial_board[i][j] = Player::piece::e : initial_board[i][j] = Player::piece::o;
            else
                initial_board[i][j] = Player::piece::e;
        }
    }

    std::fstream file;
    file.open(filename, std::fstream::out);

    for(int i = BOARD_SIZE - 1; i >= 0; i--) {
        for(int j = 0; j < BOARD_SIZE; j++) {
            file << convert_to_char(initial_board[i][j]);
            if(j != BOARD_SIZE - 1) file << " ";
        }
        if(i != 0)
            file << "\n";
    }

    file.close();
    // deletes the temporary variables

    delete_board(initial_board);
}

/*
 * make a move starting from the most recent board in the history.
 * move one piece of this player, and possibly remove pieces of the other
 * player if they have been eaten.
 * The new board is appended at the end of the history.
 * An empty move (no move) is not admissible: if an empty move is appended, the game
 * is automatically lost.
  */
void Player::move(){
    std::cout << "move called" << std::endl;
    Move temp_moves;
    int arr_size = 0;
    auto available_pieces = temp_moves.get_available_pieces(this->pimpl->player_nr, this->pimpl->board, arr_size);
    std::cout << "arr_size: " << arr_size << std::endl;
    //Move* moves_list = new Move[arr_size];
    delete[] available_pieces;
}
/**
 * compares the latest two boards and checks if the move is valid
 * @return true if the move is valid, false otherwise
 */
bool Player::valid_move() const{
    std::cout << "valid_move called" << std::endl;
    return true;
}
/**
 * deletes the latest board in the player memory
 */
void Player::pop(){
    std::cout << "pop called" << std::endl;
    if(this->pimpl->next == nullptr){
        if(this->pimpl->board == nullptr)
            throw player_exception{player_exception::index_out_of_bounds, "The board is empty"};
        delete_board(this->pimpl->board);
        this->pimpl->board = nullptr;
    }
    else {
        Impl* temp = this->pimpl;
        while(temp->next->next != nullptr)
            temp = temp->next;
        if(temp->next->board != nullptr)
            delete_board(temp->next->board);
        delete temp->next;
        temp->next = nullptr;
    }
}

/**
 * checks if the player has won the game
 * @param player_nr the player to check
 * @return true if the player has won the game, false otherwise
 */
bool Player::wins(int player_nr) const{
    std::cout << "wins called" << std::endl;
    return true;
}

/**
 * checks if the latest board has won the game
 * @return true if the latest board has won the game, false otherwise
 */
bool Player::wins() const{
    std::cout << "wins called" << std::endl;
    return true;
}

/**
 * checks if the selected player has lost the game
 * @param player_nr the player to check
 * @return true if the player has lost the game, false otherwise
 */
bool Player::loses(int player_nr) const{
    std::cout << "loses called" << std::endl;
    return true;
}

/**
 * checks if the latest bord has lost the game
 * @return true if the latest bord has lost the game, false otherwise
 */
bool Player::loses() const{
    std::cout << "loses called" << std::endl;
    return true;
}

/**
 * return how many times the last board appeared in the past
 * for example, if the history is A B C B D C B (where A,B,C,D are distinct boards and the most recent is 'B')
 * then this function returns 3 because the most recent board (B) appears 3 times
 * in the history
 * @return the number of boards
 */
int Player::recurrence() const{
    std::cout << "recurrence called" << std::endl;
    return 0;
}

int main(){
    try {
        Player p1(1);
        //p1.init_board("./stored_board.txt");
        p1.load_board("./stored_board.txt");
        //p1.store_board("./stored_board.txt", 1);
    }
    catch(player_exception& e){
        std::cout << e.msg << std::endl;
    }
    return 0;
}