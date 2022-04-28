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

int main(int argc, char **argv){
	
	int player_nr = atoi(argv[1]);	
	//Player p(player_nr);
	
	cout << "Playing as player " << player_nr << endl;
	
	int round = player_nr; // 1 or 2
		
	while(true){
		
		string board_name =  "board_" + std::to_string(round) + ".txt";
		ifstream infile(board_name);
		
		if(infile.good()){
		
			cout << "Reading board " << board_name << endl;
		
			infile.close();
			std::this_thread::sleep_for (std::chrono::milliseconds(100));
			
			p.load_board(board_name);
			p.move();
			board_name =  "board_" + std::to_string(++round) + ".txt";
			p.store_board(board_name);
			round++;
			
		}
		
	}
	
}
