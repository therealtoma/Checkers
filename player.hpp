// Do not include anything else! all other #include's will be deleted.
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>

struct player_exception{
	enum err_type {index_out_of_bounds,missing_file,invalid_board};
	err_type t;
	std::string msg;
};

class Player{
	
public:
	
	/* Both players begin with all "Pedina" pieces: player 1 has x and player 2 has o
	 * when a "Pedina" reaches the farthest row forward, becomes a "Dama" (X or O). 
	 * type 'e' is used for empty board cells. 
	 */
	enum piece {x, o, X, O, e};
	
	/*
	 * start player with empty history. player_nr can be either 1 or 2. 
	 * Player 1 is the one starting in the low row values (rows 0,1,2), 
	 * player 2 starts in the high row values (rows 5,6,7).
	 */ 
	Player(int player_nr = 1);
	~Player(); //remember to release the allocated memory!
	
	Player(const Player&); //copy constructor
	
	Player& operator=(const Player&); //copy assignment

	/*
	 * piece contained at cell (r,c) (row-column) in the history_offset-th board in the past 
	 * (i.e. if history_offset = 0, then the board is the most recent one)
	 */
	piece operator()(int r, int c, int history_offset = 0) const;
	
	/* load board from file and append it at the end of the history.
	 * Suggestion: store the history in a structure efficiently supporting adding a new board!
	 * an array of boards is good only if you re-allocate using a doubling technique. 
	 * Better to use a list of boards.  
	 */
	void load_board(const string& filename);

	/*
	 * save the history_offset-th board (counting from most to least recent) to file
	 * (if history_offset = 0, then the board is the most recent one)
	 */
	void store_board(const string& filename, int history_offset = 0) const;
	
	//create and store an initial board to file
	void init_board(const string& filename) const;
	
	/* 
	 * make a move starting from the most recent board in the history.
	 * move one piece of this player, and possibly remove pieces of the other
	 * player if they have been eaten.
	 * The new board is appended at the end of the history.
	 * An empty move (no move) is not admissible: if an empty move is appended, the game
	 * is automatically lost.
 	 */
	void move();
	
	// Compare the latest two boards in history and decide if the move is valid.
	bool valid_move() const;

	//removes the latest board from history
	void pop();

	//is the latest board winning for player_nr (1 or 2)?
	bool wins(int player_nr) const;

	//is the latest board winning for me? (player number player_nr)
	bool wins() const;

	bool loses(int player_nr) const;

	bool loses() const;

	/* return how many times the last board appeared in the past
	 * for example, if the history is A B C B D C B (where A,B,C,D are distinct boards and the most recent is 'B')
	 * then this function returns 3 because the most recent board (B) appears 3 times
	 * in the history
	 */
	int recurrence() const;
	
private:

	struct Impl;
	Impl* pimpl;
		
};
