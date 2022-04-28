//#include "player.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <thread>

using std::cout;
using std::cin;
using std::endl;
using std::ifstream;
using std::string;

constexpr int MAX_ROUNDS = 1000;

int main(int argc, char **argv){
	
	//create new verifier
	Player v();
	
	cout << "Running verifier" << endl;
	
	string board_name =  "board_1.txt";		
	v.init_board(board_name);
	
	int round = 2;
	
	while(true){

		board_name =  "board_" + std::to_string(round) + ".txt";
		ifstream infile(board_name);
		
		if(infile.good()){
		
			if(round >= MAX_ROUNDS){
					cout << "Game over. Too many rounds. Both players lose." << endl;
					exit(0);
			}
		
			int player_nr = round%2 == 0 ? 1 : 2;//player that created board_name
	
			cout << "Verifying board " << board_name << " created by Player " << player_nr << endl;
		
			infile.close();
			std::this_thread::sleep_for (std::chrono::milliseconds(100));
			
			v.load_board(board_name);
			
			if(v.valid_move()){
			
				if(p.wins(player_nr)){
					cout << "Game over. Player " << player_nr << " wins." << endl;
					exit(0);
				}
						
				round++;
				
			}else{
				//invalid move. End game and declare that player player_nr lost due to invalid move.
				cout << "Game over: invalid move by Player " << player_nr << ". Player " << (player_nr==1?2:1) << " wins." << endl;
				exit(0);
			}
				
		}
		
	}
	
}
