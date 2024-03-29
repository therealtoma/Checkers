#include "player.hpp"

#define BOARD_SIZE 8
#define NUMBER_OF_x 12
#define NUMBER_OF_o 12
/**
 * converts a piece into char
 * @param p the piece to convert
 * @return the converted character '0' in case of error
 */
char convert_to_char(Player::piece p)
{
	switch (p)
	{
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
	throw player_exception{player_exception::invalid_board, "EXCEPTION: The insered piece is not valid"};
}

/**
 * converts a piece into a string
 * @param c the character to convert
 * @return tha converted piece, e for default
 */
Player::piece convert_to_piece(char c)
{
	switch (c)
	{
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
	throw player_exception{player_exception::invalid_board, "EXCEPTION: The inserted character is not valid, received: " + std::to_string(c)};
}

/**
 * converts a piece into a checker, the empty space stays default, if the piece does not exist an exception is thrown
 * @param p the piece to convert
 * @return the corrisponding checker piece
 */
Player::piece convert_to_checker(Player::piece p) {
    switch (p) {
        case Player::piece::x:
            return Player::piece::X;
        case Player::piece::o:
            return Player::piece::O;
        case Player::piece::X:
            return Player::piece::x;
        case Player::piece::O:
            return Player::piece::o;
        case Player::piece::e:
            return Player::piece::e;
    }

    throw player_exception{player_exception::invalid_board, "ERROR: the passed piece is not valid"};

}

/**
 * allocates the memory for a board
 * @return the allocated board
 */
Player::piece **initialize_board()
{
	Player::piece **matrix = nullptr;
	matrix = new Player::piece *[BOARD_SIZE];
	for (int i = 0; i < BOARD_SIZE; i++)
		matrix[i] = new Player::piece[BOARD_SIZE];

	return matrix;
}

/**
 * deallocates the memory for the passed board
 * @param board the boad to deallocate
 */
void delete_board(Player::piece **(&board))
{
	for (int i = 0; i < BOARD_SIZE; i++)
		delete[] board[i];
	delete[] board;
}
/**
 * prints the board
 * @param board the board to be printed
 */
void print_board(Player::piece **(&board))
{
	std::cout << "----------------" << std::endl;
	for (int i = BOARD_SIZE - 1; i >= 0; i--)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
			std::cout << convert_to_char(board[i][j]);
		std::cout << std::endl;
	}
	std::cout << "----------------" << std::endl;
}

/**
 * checks whether a file exists
 * @param filename the name of the file to check
 * @return true if the file exists, false otherwise
 */
bool file_exists(const std::string &filename)
{
	std::ifstream f(filename.c_str());
	return f.good();
}

/**
 * finds the highest value in thge array
 * @param arr tha array
 * @param size the size of the array
 * @return the hisghest value in the array
 */
int find_max(int arr[], int size)
{
	int max = arr[0];

	for (int i = 0; i < size; i++)
	{
		if (arr[i] > max)
			max = arr[i];
        else if (arr[i] == max)
            max = ((rand() % 100 + 1) % 2 == 0) ? max : arr[i];
	}

	return max;
}

/**
 * count the number of pieces left in the specified borad of the specified player
 * @param board the board to check
 * @param player_nr the player number
 * @return the number of pieces left
 */
int count_pieces(Player::piece** board, int player_nr) {

    Player::piece piece_to_check = (player_nr == 1) ? Player::piece::o : Player::piece::x;
    Player::piece dame_to_check = (player_nr == 1) ? Player::piece::O : Player::piece::X;

    int n_pieces = 0;

    for(int i = 0; i < BOARD_SIZE; i++){
        for(int j = 0; j < BOARD_SIZE; j++) {
            if(board[i][j] == piece_to_check || board[i][j] == dame_to_check) n_pieces++;
        }
    }

    return n_pieces;
}
// struct Move code
struct Move
{
	std::pair<int, int> current_position;
	std::pair<int, int> *available_moves;
	std::pair<std::pair<int, int>, int> *evaluations;
	Player::piece piece;
	int length;

	enum evaluations
	{
		empty_move,
		become_checker,
		eat_piece,
		eat_checker,
		eat_piece_and_checker
	};

	/**
	 * gets an array of pair of all the positions of a specific piece of a specific board of a specific player
	 * @param player_nr the player number
	 * @param board the board to chekc
	 * @param arr_size the size of the returned array
	 * @return an array of pair with the positions
	 * @throws player_exception::board_not_valid if the board is not valid
	 * @throws player_exception::index_out_of_bounds in case the player number is not valid
	 */
	std::pair<int, int> *get_available_pieces(int player_nr, Player::piece **board, int &arr_size)
	{
		// getting the pieces to find based on the player number
		Player::piece piece_to_find = (player_nr == 1) ? Player::piece::x : Player::piece::o;
		Player::piece dame_to_find = (player_nr == 1) ? Player::piece::X : Player::piece::O;
		arr_size = 0;
		// creating the array

		// finding the number of pieces to insert
		for (int i = 0; i < BOARD_SIZE; i++)
		{
			for (int j = 0; j < BOARD_SIZE; j++)
			{
				if (board[i][j] == piece_to_find || board[i][j] == dame_to_find)
					arr_size++;
			}
		}

		auto valid_positions = new std::pair<int, int>[arr_size];
		int count = 0;
		// i = y
		for (int i = 0; i < BOARD_SIZE; i++)
		{
			// j = x
			for (int j = 0; j < BOARD_SIZE; j++)
			{
				if (board[i][j] == piece_to_find || board[i][j] == dame_to_find)
				{
					valid_positions[count] = std::make_pair(i, j);
					count++;
				}
			}
		}

		// returning the array
		return valid_positions;
	}

	/**
	 * finds all the possible positions the selected piece can go to
	 * @param position the position to consider
	 * @param board the board to check
	 * @param size the size of the positions list
	 */
	void get_available_moves(std::pair<int, int> position, Player::piece **board, int &size)
	{
		// if the received position is not valid an exception is thrown
		if (position.first < 0 || position.second < 0 || position.first >= BOARD_SIZE || position.second >= BOARD_SIZE)
			throw player_exception{
				player_exception::index_out_of_bounds,
				"ERROR: The inserted position in the get_available_moves function is not correct. Received coords: [" +
					std::to_string(position.first) + ", " + std::to_string(position.second) + "]"};

		// if the chosen position is an empty space the moves list is set to nullptr and the function ends
		if (board[position.first][position.second] == Player::piece::e)
		{
			this->available_moves = nullptr;
			this->evaluations = nullptr;
			return;
		}

		// finding out if the piece is a checker or not
		bool is_checker = (this->piece == Player::piece::X || this->piece == Player::piece::O);
		int player_nr = (this->piece == Player::piece::X || this->piece == Player::piece::x) ? 1 : 2;

		// setting helpful variables to default values
		Player::piece checker_piece = Player::piece::e, normal_piece = Player::piece::e, enemy_checker = Player::piece::e;
		int total_possible_moves = 0, actual_moves = 0;

		// finding the player_nr and changing variables accordingly
		if (player_nr == 1)
		{
			normal_piece = Player::piece::x;
			checker_piece = Player::piece::X;
			enemy_checker = Player::piece::O;
		}
		else
		{
			normal_piece = Player::piece::o;
			checker_piece = Player::piece::O;
			enemy_checker = Player::piece::X;
		}

		// the piece is a checker
		if (is_checker)
		{
			total_possible_moves = 4;
			this->available_moves = new std::pair<int, int>[total_possible_moves];
			this->evaluations = new std::pair<std::pair<int, int>, int>[total_possible_moves];

			// check if it can go top-left
			if (position.first + 1 < BOARD_SIZE && position.second - 1 >= 0)
			{
				// can't eat
				if (board[position.first + 1][position.second - 1] == Player::piece::e)
				{
					this->available_moves[actual_moves] = std::make_pair(position.first + 1, position.second - 1);
					// the checker made a normal move
					this->evaluations[actual_moves] = std::make_pair(this->available_moves[actual_moves], empty_move);
					actual_moves++;
				}
				else
				{
					// making sure we don't go outside the board
					if (position.first + 2 < BOARD_SIZE && position.second - 2 >= 0)
					{
						// the eatable piece is not a player's piece
						if (board[position.first + 1][position.second - 1] != normal_piece &&
							board[position.first + 1][position.second - 1] != checker_piece)
						{
							// the final position is an empty space
							if (board[position.first + 2][position.first - 2] == Player::piece::e)
							{
								this->available_moves[actual_moves] = std::make_pair(position.first + 2,
																					 position.second - 2);
								// evaluating the move in in case we eat a piece or a checker
								(board[position.first + 1][position.second - 1] == enemy_checker)
									? this->evaluations[actual_moves] = std::make_pair(this->available_moves[actual_moves], eat_checker)
									: this->evaluations[actual_moves] = std::make_pair(this->available_moves[actual_moves], eat_piece);

								actual_moves++;
							}
						}
					}
				}
			}

			// check if it can go top-right
			if (position.first + 1 < BOARD_SIZE && position.second + 1 < BOARD_SIZE)
			{
				// can't eat
				if (board[position.first + 1][position.second + 1] == Player::piece::e)
				{
					this->available_moves[actual_moves] = std::make_pair(position.first + 1, position.second + 1);
					// the checker made a normal move
					this->evaluations[actual_moves] = std::make_pair(this->available_moves[actual_moves], empty_move);
					actual_moves++;
				}
				else
				{
					// making sure we don't go outside the board
					if (position.first + 2 < BOARD_SIZE && position.second + 2 < BOARD_SIZE)
					{
						// the eatable piece is not a player's piece
						if (board[position.first + 1][position.second + 1] != normal_piece &&
							board[position.first + 1][position.second + 1] != checker_piece)
						{
							// the final position is an empty space
							if (board[position.first + 2][position.second + 2] == Player::piece::e)
							{
								this->available_moves[actual_moves] = std::make_pair(position.first + 2,
																					 position.second + 2);
								// evaluating the move in case we eat a piece or a checker
								(board[position.first + 1][position.second + 1] == enemy_checker)
									? this->evaluations[actual_moves] = std::make_pair(this->available_moves[actual_moves], eat_checker)
									: this->evaluations[actual_moves] = std::make_pair(this->available_moves[actual_moves], eat_piece);

								actual_moves++;
							}
						}
					}
				}
			}

			// check if it can go bottom-right
			if (position.first - 1 >= 0 && position.second + 1 < BOARD_SIZE)
			{
				// can't eat
				if (board[position.first - 1][position.second + 1] == Player::piece::e)
				{
					this->available_moves[actual_moves] = std::make_pair(position.first - 1, position.second + 1);
					this->evaluations[actual_moves] = std::make_pair(this->available_moves[actual_moves], empty_move);
					actual_moves++;
				}
				else
				{
					// making sure we don't go outside the board
					if (position.first - 2 >= 0 && position.second + 2 < BOARD_SIZE)
					{
						// the eatable piece is not a player's piece
						if (board[position.first - 1][position.second + 1] != normal_piece &&
							board[position.first - 1][position.second + 1] != checker_piece)
						{
							// the final position is an empty space
							if (board[position.first - 2][position.second + 2] == Player::piece::e)
							{
								this->available_moves[actual_moves] = std::make_pair(position.first - 2,
																					 position.second + 2);

								// evaluating the move in case we eat a piece or a checker
								(board[position.first - 1][position.second + 1] == enemy_checker)
									? this->evaluations[actual_moves] = std::make_pair(this->available_moves[actual_moves], eat_checker)
									: this->evaluations[actual_moves] = std::make_pair(this->available_moves[actual_moves], eat_piece);
								actual_moves++;
							}
						}
					}
				}
			}

			// check if it can go bottom-left
			if (position.first - 1 >= 0 && position.second - 1 >= 0)
			{
				// can't eat
				if (board[position.first - 1][position.second - 1] == Player::piece::e)
				{
					this->available_moves[actual_moves] = std::make_pair(position.first - 1, position.second - 1);
					this->evaluations[actual_moves] = std::make_pair(this->available_moves[actual_moves], empty_move);
					actual_moves++;
				}
				else
				{
					// making sure we don't go outside the board
					if (position.first - 2 >= 0 && position.second - 2 >= 0)
					{
						// the eatable piece is not a player's piece
						if (board[position.first - 1][position.second - 1] != normal_piece &&
							board[position.first - 1][position.second - 1] != checker_piece)
						{
							// the final position is an empty space
							if (board[position.first - 2][position.second - 2] == Player::piece::e)
							{
								this->available_moves[actual_moves] = std::make_pair(position.first - 2,
																					 position.second - 2);

								// evaluating move in case it eat a piece or a checker
								(board[position.first - 1][position.second - 1] == enemy_checker)
									? this->evaluations[actual_moves] = std::make_pair(this->available_moves[actual_moves], eat_checker)
									: this->evaluations[actual_moves] = std::make_pair(this->available_moves[actual_moves], eat_piece);

								actual_moves++;
							}
						}
					}
				}
			}
		}
		// it is a normal piece
		else
		{
			total_possible_moves = 2;
			this->available_moves = new std::pair<int, int>[total_possible_moves];
			this->evaluations = new std::pair<std::pair<int, int>, int>[total_possible_moves];
			// player_nr == 1 the piece can only go top
			if (player_nr == 1)
			{
				// cheking if it can go top-right
				if (position.first + 1 < BOARD_SIZE && position.second + 1 < BOARD_SIZE)
				{
					if (board[position.first + 1][position.second + 1] == Player::piece::e)
					{
						this->available_moves[actual_moves] = std::make_pair(position.first + 1,
																			 position.second + 1);
						(position.first + 1 == BOARD_SIZE - 1)
							? this->evaluations[actual_moves] = std::make_pair(this->available_moves[actual_moves], become_checker)
							: this->evaluations[actual_moves] = std::make_pair(this->available_moves[actual_moves], empty_move);

						actual_moves++;
					}
					else
					{
						if (position.first + 2 < BOARD_SIZE && position.second + 2 < BOARD_SIZE)
						{
							if (board[position.first + 1][position.second + 1] != enemy_checker)
							{
								if (board[position.first + 1][position.second + 1] != checker_piece
                                && board[position.first + 1][position.second + 1] != normal_piece
                                && board[position.first + 2][position.second + 2] == Player::piece::e)
								{
									this->available_moves[actual_moves] = std::make_pair(position.first + 2,
																						 position.second + 2);

									(position.first + 2 == BOARD_SIZE - 1)
										? this->evaluations[actual_moves] = std::make_pair(this->available_moves[actual_moves], eat_piece_and_checker)
										: this->evaluations[actual_moves] = std::make_pair(this->available_moves[actual_moves], eat_piece);

									actual_moves++;
								}
							}
						}
					}
				}
				// cheking if it can go top-left
				if (position.first + 1 < BOARD_SIZE && position.second - 1 >= 0)
				{
					if (board[position.first + 1][position.second - 1] == Player::piece::e)
					{
						this->available_moves[actual_moves] = std::make_pair(position.first + 1,
																			 position.second - 1);
						(position.first + 1 == BOARD_SIZE - 1)
							? this->evaluations[actual_moves] = std::make_pair(this->available_moves[actual_moves], become_checker)
							: this->evaluations[actual_moves] = std::make_pair(this->available_moves[actual_moves], empty_move);

						actual_moves++;
					}
					else
					{
						if (position.first + 2 < BOARD_SIZE && position.second - 2 >= 0)
						{
							if (board[position.first + 1][position.second - 1] != enemy_checker)
							{
								if (board[position.first + 1][position.second - 1] != checker_piece
                                && board[position.first + 1][position.second - 1] != normal_piece
                                && board[position.first + 2][position.second - 2] == Player::piece::e)
								{
									this->available_moves[actual_moves] = std::make_pair(position.first + 2,
																						 position.second - 2);
									(position.first + 2 == BOARD_SIZE - 1)
										? this->evaluations[actual_moves] = std::make_pair(this->available_moves[actual_moves], eat_piece_and_checker)
										: this->evaluations[actual_moves] = std::make_pair(this->available_moves[actual_moves], eat_piece);

									actual_moves++;
								}
							}
						}
					}
				}
			}
			// player_nr == 2 the piece can only go bottom
			else
			{
				// chacking if it can go bottom-right
				if (position.first - 1 >= 0 && position.second + 1 < BOARD_SIZE){
					if (board[position.first - 1][position.second + 1] == Player::piece::e)
					{
						this->available_moves[actual_moves] = std::make_pair(position.first - 1,
																			 position.second + 1);
						(position.first - 1 == 0)
							? this->evaluations[actual_moves] = std::make_pair(this->available_moves[actual_moves], become_checker)
							: this->evaluations[actual_moves] = std::make_pair(this->available_moves[actual_moves], empty_move);

						actual_moves++;
					}
					else
					{
						if (position.first - 2 >= 0 && position.second + 2 < BOARD_SIZE)
						{
							if (board[position.first - 1][position.second + 1] != enemy_checker)
							{
								if (board[position.first - 1][position.second + 1] != checker_piece
                                && board[position.first - 1][position.second + 1] != normal_piece
                                && board[position.first - 2][position.second + 2] == Player::piece::e)
								{
									this->available_moves[actual_moves] = std::make_pair(position.first - 2,
																						 position.second + 2);

									(position.first - 2 == 0)
										? this->evaluations[actual_moves] = std::make_pair(this->available_moves[actual_moves], eat_piece_and_checker)
										: this->evaluations[actual_moves] = std::make_pair(this->available_moves[actual_moves], eat_piece);

									actual_moves++;
								}
							}
						}
					}
				}
				// checking if it can go bottom-left
				if (position.first - 1 >= 0 && position.second - 1 >= 0){
					if (board[position.first - 1][position.second - 1] == Player::piece::e)
					{
						this->available_moves[actual_moves] = std::make_pair(position.first - 1,
																			 position.second - 1);

						(position.first - 1 == 0)
							? this->evaluations[actual_moves] = std::make_pair(this->available_moves[actual_moves], become_checker)
							: this->evaluations[actual_moves] = std::make_pair(this->available_moves[actual_moves], empty_move);
						actual_moves++;
					}
					else
					{
						if (position.first - 2 >= 0 && position.second - 2 >= 0)
						{
							if (board[position.first - 1][position.second - 1] != enemy_checker)
							{
								if (board[position.first - 1][position.second - 1] != checker_piece
                                && board[position.first - 1][position.second - 1] != normal_piece
                                && board[position.first - 2][position.second - 2] == Player::piece::e)
								{
									this->available_moves[actual_moves] = std::make_pair(position.first - 2,
																						 position.second - 2);

									(position.first - 2 == 0)
										? this->evaluations[actual_moves] = std::make_pair(this->available_moves[actual_moves], eat_piece_and_checker)
										: this->evaluations[actual_moves] = std::make_pair(this->available_moves[actual_moves], eat_piece);

									actual_moves++;
								}
							}
						}
					}
				}
			}
		}
		if (actual_moves == 0)
		{
			delete[] this->available_moves;
			delete[] this->evaluations;
			this->evaluations = nullptr;
			this->available_moves = nullptr;
		}
		else if (actual_moves != total_possible_moves)
		{
			auto temp_moves = new std::pair<int, int>[actual_moves];
			auto temp_eval = new std::pair<std::pair<int, int>, int>[actual_moves];
			for (int i = 0; i < actual_moves; i++)
			{
				temp_moves[i] = this->available_moves[i];
				temp_eval[i] = this->evaluations[i];
			}
			delete[] this->evaluations;
			delete[] this->available_moves;
			this->available_moves = new std::pair<int, int>[actual_moves];
			this->evaluations = new std::pair<std::pair<int, int>, int>[actual_moves];
			for (int i = 0; i < actual_moves; i++)
			{
				this->available_moves[i] = temp_moves[i];
				this->evaluations[i] = temp_eval[i];
			}
			delete[] temp_eval;
			delete[] temp_moves;
		}

		size = actual_moves;
		length = actual_moves;
	}

	/**
	 * finds the position with the best possible move for a specific Move struct, in case of two equal moves a random one is chosen
	 * @param size the size of the evaluations array
	 * @return the position in the evaluations array with the best possible move, -1 in case the eavaluations array is empty
	 */
	int find_best_move()
	{
		// if the array is empty we return -1
		if (this->evaluations == nullptr)
			return -1;

		// the index with the highest score
		int max = 0;

		for (int i = 0; i < this->length; i++)
		{
			// the current value
			int temp_val = this->evaluations[i].second;
			// the value with the current highest score
			int max_val = this->evaluations[max].second;

			// updating the max value
			if (temp_val > max_val)
				max = i;
			// if the two values are the same, we chose a random one
			else if (max_val == temp_val)
				max = ((rand() % 100 + 1) % 2 == 0) ? i : max;
		}
		return max;
	}
};
// end struct Move code
struct Player::Impl
{
	Impl *next;
	Player::piece **board; // the dama board
	int index;			   // the index of the board
	int player_nr;		   // the player number
};

/**
 * Construct a new Player object
 * @param player_nr the player number
 */
Player::Player(int player_nr)
{

	// checks if player number is valid otherwise throws an exception
	if (player_nr != 1 && player_nr != 2)
		throw player_exception{player_exception::index_out_of_bounds, "EXCEPTION: The player number can only be 1 or 2. Received " + std::to_string(player_nr)};

	pimpl = new Impl{nullptr, nullptr, 0, player_nr}; // initializes the memory

} // constructor

/**
 * Destroy the Player object
 */
Player::~Player()
{
	// loops the list
	while (pimpl != nullptr)
	{
		Impl *temp = pimpl;	 // saves the list address
		pimpl = pimpl->next; // goes to the next node
		if (temp->board != nullptr)
		{
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
Player::Player(const Player &copy)
{
	if (copy.pimpl->board != nullptr)
	{
		this->pimpl = new Impl{
			nullptr,
			initialize_board(),
			copy.pimpl->index,
			copy.pimpl->player_nr};

		for (int i = 0; i < BOARD_SIZE; i++)
		{
			for (int j = 0; j < BOARD_SIZE; j++)
			{
				this->pimpl->board[i][j] = copy.pimpl->board[i][j];
			}
		}

		Impl *copy_temp = copy.pimpl;
		Impl *this_temp = this->pimpl;

		while (copy_temp->next)
		{
			this_temp->next = new Impl{
				nullptr,
				initialize_board(),
				this_temp->index + 1,
				copy_temp->player_nr};

			for (int i = 0; i < BOARD_SIZE; i++)
			{
				for (int j = 0; j < BOARD_SIZE; j++)
				{
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
Player::piece Player::operator()(int r, int c, int history_offset) const
{

	// checkin if the row is within the range 0 <= r < BOARD_SIZE
	if (r >= BOARD_SIZE || r < 0)
		throw player_exception{player_exception::index_out_of_bounds, "EXCEPTION: The inserted row is not valid. Reveived " + std::to_string(r)};

	// checkin if the row is within the range 0 <= r < BOARD_SIZE
	if (c >= BOARD_SIZE || c < 0)
		throw player_exception{player_exception::index_out_of_bounds, "EXCEPTION: The inserted column is not valid. Received " + std::to_string(c)};

	Impl *temp = this->pimpl;
	int memory_size = 0;
	(this->pimpl->board != nullptr) ? memory_size = 1 : throw player_exception{player_exception::index_out_of_bounds, "The inserted history_offset is not valid"};
	// calculating memory size
	while (temp->next)
	{
		temp = temp->next;
		memory_size++;
	}

	// checking history_offset validity
	if (history_offset >= memory_size)
		throw player_exception{player_exception::index_out_of_bounds, "EXCEPTION: The inserted history_offset is not valid. Received " + std::to_string(history_offset)};

	// calculating the index of the chosen board
	int index = memory_size - 1;
	temp = this->pimpl;

	// going to the chosen board
	while (index != history_offset)
	{
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
Player &Player::operator=(const Player &p)
{
	// avoiding self assignment
	if (&p != this)
	{
		while (this->pimpl)
		{
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
			p.pimpl->player_nr};

		for (int i = 0; i < BOARD_SIZE; i++)
		{
			for (int j = 0; j < BOARD_SIZE; j++)
			{
				this->pimpl->board[i][j] = p.pimpl->board[i][j];
			}
		}

		Impl *copy_temp = p.pimpl;
		Impl *this_temp = this->pimpl;

		while (copy_temp->next)
		{
			this_temp->next = new Impl{
				nullptr,
				initialize_board(),
				this_temp->index + 1,
				copy_temp->player_nr};

			for (int i = 0; i < BOARD_SIZE; i++)
			{
				for (int j = 0; j < BOARD_SIZE; j++)
				{
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
void Player::load_board(const std::string &filename)
{
	Impl *temp = this->pimpl;
	int last_index = this->pimpl->index;

	if (this->pimpl->board == nullptr)
	{
		this->pimpl->board = initialize_board();
		temp = this->pimpl;
	}
	else
	{
		// goes to the end of the player list
		while (temp->next)
		{
			last_index++;
			temp = temp->next;
		}
		temp->next = new Impl{
			nullptr,
			initialize_board(),
			last_index + 1,
			this->pimpl->player_nr};
		temp = temp->next;
	}

	if (!file_exists(filename))
		throw player_exception{player_exception::missing_file, "EXCEPTION: file " + filename + " does not exist"};

	std::fstream file(filename, std::fstream::in);
	Player::piece **board = initialize_board();
	char cella;
	int read_characters = 0, i = BOARD_SIZE - 1, j = 0, count_x = 0, count_o = 0;

	while (file.get(cella))
	{
		if (cella != '\n')
		{
			// the file containes too many characters
			if (i < 0)
			{
				delete_board(board);
				throw player_exception{player_exception::invalid_board,
									   "EXCEPTION: the board (" + filename + ") containes more values than it should."};
			}
			// a piece is in the white space
			if ((i + j) % 2 == 0 && cella != ' ')
			{
				delete_board(board);
				throw player_exception{player_exception::invalid_board,
									   "EXCEPTION: there's a piece in a not allowed space. Reading file " + filename};
			}
			// counts the number of o's and x's
			if (cella == 'o' || cella == 'O')
				count_o++;
			if (cella == 'x' || cella == 'X')
				count_x++;
			board[i][j] = convert_to_piece(cella);
			j++;
			read_characters++;
			if (j == BOARD_SIZE)
			{
				j = 0;
				i--;
			}
		}
		file.get(cella);
	}
	file.close();

	if (read_characters != BOARD_SIZE * BOARD_SIZE)
	{
		delete_board(board);
		throw player_exception{player_exception::invalid_board, "EXCEPTION: the selected board (" + filename + ") is not valid. The problem is about it's size."};
	}

	if (count_x > NUMBER_OF_x)
	{
		delete_board(board);
		throw player_exception{player_exception::invalid_board, "EXCEPTION: there are too many x pieces in the board (" + filename + ")"};
	}

	if (count_o > NUMBER_OF_o)
	{
		delete_board(board);
		throw player_exception{player_exception::invalid_board, "EXCEPTION: there are too many o pieces in the board (" + filename + ")"};
	}

	for (i = 0; i < BOARD_SIZE; i++)
	{
		for (j = 0; j < BOARD_SIZE; j++)
		{
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
void Player::store_board(const std::string &filename, int history_offset) const
{
	Impl *temp = this->pimpl;
	int memory_size = 0;

	while (temp->next != nullptr)
	{
		temp = temp->next;
		memory_size++;
	}

	if (history_offset >= memory_size)
		throw player_exception{player_exception::index_out_of_bounds, "EXCEPTION: The inserted history_offset is not valid. Received " + std::to_string(history_offset)};

	int index = memory_size - history_offset;
	temp = this->pimpl;

	while (index != 0)
	{
		temp = temp->next;
		index--;
	}

	std::fstream file;
	file.open(filename, std::fstream::out);

	for (int i = BOARD_SIZE - 1; i >= 0; i--)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			file << convert_to_char(temp->board[i][j]);
			if (j != BOARD_SIZE - 1)
				file << ' ';
		}
		if (i != 0)
			file << "\n";
	}

	file.close();

}

/**
 * creates an initial board and stores it inside the first memory of the player and inside the file
 * @param filename the file to to store the board into
 */
void Player::init_board(const std::string &filename) const
{
	// initial board

	// allocates the memory
	Player::piece **initial_board = initialize_board();

    // going to the end of the player memory
    Impl* temp = this->pimpl;
    while(temp->next != nullptr)
        temp = temp->next;

    // creating a new memory space
    temp->next = new Impl {
        nullptr,
        initialize_board(),
        temp->index+1,
        this->pimpl->player_nr
    };

	// fill starting board with the default field
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			if (i >= 0 && i <= 2)
				((i + j) % 2) == 0 ? initial_board[i][j] = Player::piece::e : initial_board[i][j] = Player::piece::x;
			else if (i >= 5 && i <= 7)
				((i + j) % 2) == 0 ? initial_board[i][j] = Player::piece::e : initial_board[i][j] = Player::piece::o;
			else
				initial_board[i][j] = Player::piece::e;
            // updating the initial board
            temp->next->board[i][j] = initial_board[i][j];
		}
	}

	std::fstream file;
	file.open(filename, std::fstream::out);

	for (int i = BOARD_SIZE - 1; i >= 0; i--)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			file << convert_to_char(initial_board[i][j]);
			if (j != BOARD_SIZE - 1)
				file << " ";
		}
		if (i != 0)
			file << "\n";
	}

	file.close();
	// deletes the temporary variables

	delete_board(initial_board);
}

/**
 * make a move starting from the most recent board in the history.
 * move one piece of this player, and possibly remove pieces of the other
 * player if they have been eaten.
 * The new board is appended at the end of the history.
 * An empty move (no move) is not admissible: if an empty move is appended, the game
 * is automatically lost.
 */
void Player::move()
{
    // we throw an exception in case the player contains no boards
    if(this->pimpl->board == nullptr)
        throw player_exception{player_exception::index_out_of_bounds, "ERROR: the move can't operate because the player contains no boards"};

    // pick the last board
    Impl* temp = this->pimpl;
    while(temp->next != nullptr) {
        temp = temp->next;
    }

    // declaring some variables
	Move temp_moves;
	int arr_size = 0, available_moves_size = 0;

	// gets the list of the available pieces
	auto available_pieces = temp_moves.get_available_pieces(this->pimpl->player_nr, temp->board, arr_size);


	// creates the array of the moves
	auto moves_list = new Move[arr_size];
    auto best_evaluations = new std::pair<std::pair<int, int>, int>[arr_size];
    auto best_positions = new std::pair<int, int>[arr_size];
	// fills the array
	for (int i = 0; i < arr_size; i++)
	{
		//  initializing the list of moves
		moves_list[i] = {available_pieces[i],
						 nullptr,
						 nullptr,
						 temp->board[available_pieces[i].first][available_pieces[i].second]};

		// finding all the available moves for the current position
		moves_list[i].get_available_moves(moves_list[i].current_position, temp->board, available_moves_size);
        int temp_index = moves_list[i].find_best_move();
        best_evaluations[i] = (temp_index != -1) ? moves_list[i].evaluations[temp_index] : std::make_pair(std::make_pair(-1, -1), -1);
        best_positions[i] = (temp_index != -1) ? moves_list[i].current_position : std::make_pair(-1 ,-1);
	}
    // the variable containing the final move with its evaluation
    auto final_evaluation = best_evaluations[0];
    // the corrisponding starting position
    auto final_position = best_positions[0];

    // finding the best move to make and its corrisponding starting position
    for(int i = 1; i < arr_size; i++) {
        if(best_evaluations[i].second > final_evaluation.second) {
            final_evaluation = best_evaluations[i];
            final_position = best_positions[i];
        }
    }

    temp->next = new Impl {
        nullptr,
        initialize_board(),
        temp->index + 1,
        this->pimpl->player_nr
    };

    // copying the last board into the new one before actually making the moves
    auto moved_board = temp->next->board;
    for(int i = 0; i < BOARD_SIZE; i++) {
        for(int j = 0; j < BOARD_SIZE; j++){
            moved_board[i][j] = temp->board[i][j];
        }
    }

    Player::piece final_piece = moved_board[final_position.first][final_position.second];

    // finding what move needs to be done
    switch(final_evaluation.second) {
        // we simply move the piece
        case Move::evaluations::empty_move:
            moved_board[final_position.first][final_position.second] = Player::piece::e;
            moved_board[final_evaluation.first.first][final_evaluation.first.second] = final_piece;
            break;
            // the piece became a ckecker
        case Move::evaluations::become_checker:
            moved_board[final_position.first][final_position.second] = Player::piece::e;
            moved_board[final_evaluation.first.first][final_evaluation.first.second] = convert_to_checker(
                    final_piece);
            break;
            // we ate a piece
        case Move::evaluations::eat_piece: {
            moved_board[final_position.first][final_position.second] = Player::piece::e;
            moved_board[final_evaluation.first.first][final_evaluation.first.second] = final_piece;

            int biggest_y = (final_position.first > final_evaluation.first.first)
                            ? final_position.first - 1
                            : final_evaluation.first.first - 1;

            int biggest_x = (final_position.second > final_evaluation.first.second)
                            ? final_position.second - 1
                            : final_evaluation.first.second - 1;

            moved_board[biggest_y][biggest_x] = Player::piece::e;

            break;
        }
            // we ate a checker
        case Move::evaluations::eat_checker: {
            moved_board[final_position.first][final_position.second] = Player::piece::e;
            moved_board[final_evaluation.first.first][final_evaluation.first.second] = final_piece;

            int biggest_y = (final_position.first > final_evaluation.first.first)
                            ? final_position.first - 1
                            : final_evaluation.first.first - 1;

            int biggest_x = (final_position.second > final_evaluation.first.second)
                            ? final_position.second - 1
                            : final_evaluation.first.second - 1;

            moved_board[biggest_y][biggest_x] = Player::piece::e;

            break;
        }
            // we ate a piece and converted to a checker
        case Move::evaluations::eat_piece_and_checker:{
            moved_board[final_position.first][final_position.second] = Player::piece::e;
            moved_board[final_evaluation.first.first][final_evaluation.first.second] = convert_to_checker(final_piece);

            int biggest_y = (final_position.first > final_evaluation.first.first)
                            ? final_position.first - 1
                            : final_evaluation.first.first - 1;

            int biggest_x = (final_position.second > final_evaluation.first.second)
                            ? final_position.second - 1
                            : final_evaluation.first.second - 1;

            moved_board[biggest_y][biggest_x] = Player::piece::e;

            break;
        }
    }


	// freeing the memory
	for (int i = 0; i < arr_size; i++)
	{
		delete[] moves_list[i].available_moves;
		delete[] moves_list[i].evaluations;
	}
	delete[] available_pieces;
	delete[] moves_list;
    delete[] best_evaluations;
    delete[] best_positions;
}
/**
 * compares the latest two boards and checks if the move is valid
 * @return true if the move is valid, false otherwise
 * @throws player_exception in case the move is considered not valid
 */
bool Player::valid_move() const
{

    // controllo che esista la prima board
    if (this->pimpl == nullptr )
        throw player_exception{player_exception::index_out_of_bounds,
                               "ERROR: the the move can't be verified because the player contains less than two boards inside its memory"};
    // controllo che esista la seconda board
    if(this->pimpl->next == nullptr)
        throw player_exception{player_exception::index_out_of_bounds,
                               "ERROR: the move can't be verified because the player contains less than two boards"};
    Impl* temp = this->pimpl;
    // flag to check if the latest two boards are equal
    bool exit_check = true;
    int equality_counter = 0;

    // going to the penultimum board
    while (temp->next->next != nullptr)
        temp = temp->next;

    auto latest_board = temp->next->board;


    // looping the board
    for(int i = 0; i < BOARD_SIZE && exit_check; i++) {
        for(int j = 0; j < BOARD_SIZE && exit_check; j++) {

            // counting how many times the two board have the same pieces
            if ( temp->board[i][j] == latest_board[i][j] ) equality_counter++;

            // we are at te top of the board -> checking if a Player::piece::x gets correctly converted into Player::piece::X
            if( i == 0 )
                if( latest_board[i][j] == Player::piece::o ) exit_check = false;

            // we are at the bottom of the board -> checking if a Player::piece::o gets correctly converted into Player::piece::O
            if( i == BOARD_SIZE - 1 )
                if (latest_board[i][j] == Player::piece::x) exit_check = false;

            // if the position is dividible by 2, the piece is in a not allowed space
            if ( (i + j % 2) == 0 )
                if (latest_board[i][j] != Player::piece::e) exit_check = false;
        }
    }
    return exit_check;
}
/**
 * deletes the latest board in the player memory
 */
void Player::pop()
{
	if (this->pimpl->next == nullptr)
	{
		if (this->pimpl->board == nullptr)
			throw player_exception{player_exception::index_out_of_bounds, "EXCEPTION: The board is empty"};
		delete_board(this->pimpl->board);
		this->pimpl->board = nullptr;
	}
	else
	{
		Impl *temp = this->pimpl;
		while (temp->next->next != nullptr)
			temp = temp->next;
		if (temp->next->board != nullptr)
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
bool Player::wins(int player_nr) const
{

    if (this->pimpl->board == nullptr)
        throw player_exception{player_exception::index_out_of_bounds, "the player contains no boards"};

    if(player_nr != 1 && player_nr != 2)
        throw player_exception{player_exception::index_out_of_bounds, "The inserted player_nr is not valid"};

    Impl* temp = this->pimpl;
    while(temp->next)
        temp = temp->next;

    int n_pieces = count_pieces(temp->board, player_nr);
	return (n_pieces == 0);
}

/**
 * checks if the latest board has won the game
 * @return true if the latest board has won the game, false otherwise
 */
bool Player::wins() const
{
    return wins(this->pimpl->player_nr);
}

/**
 * checks if the selected player has lost the game
 * @param player_nr the player to check
 * @return true if the player has lost the game, false otherwise
 */
bool Player::loses(int player_nr) const
{
	return !(wins(player_nr));
}

/**
 * checks if the latest bord has lost the game
 * @return true if the latest bord has lost the game, false otherwise
 */
bool Player::loses() const
{
	return !(wins(this->pimpl->player_nr));
}

/**
 * return how many times the last board appeared in the past
 * for example, if the history is A B C B D C B (where A,B,C,D are distinct boards and the most recent is 'B')
 * then this function returns 3 because the most recent board (B) appears 3 times
 * in the history
 * @return the number of boards
 */
int Player::recurrence() const {
    // if the board is empty an exception is thrown
    if (this->pimpl->board == nullptr)
        throw player_exception{player_exception::index_out_of_bounds, "The player contains no boards"};

    // saving the first and the last board
    Impl *latest_board = this->pimpl;
    Impl *temp = this->pimpl;

    // setting variables
    bool exit = false;
    int count = 1;

    // going to the end
    while (latest_board->next)
        latest_board = latest_board->next;

    // looping
    while (temp->next) {
        for (int i = 0; i < BOARD_SIZE && !exit; i++) {
            for (int j = 0; j < BOARD_SIZE && !exit; j++) {
                // chcking if the two boards are equals
                if (latest_board->board[i][j] != temp->board[i][j]) exit = true;
            }
        }
        // if they are equal incrementing the counter
        if(!exit) count++;

        // setting variables
        exit = false;
        temp = temp->next;
    }
    // returning the result
	return count;
}
