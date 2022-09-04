/*
#include "player.hpp"
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
    //int player_nr = atoi(argv[1]);
    int player_nr = 1;
	Player p(player_nr);

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
        if(p.wins()){
            std::cout << "recurrence: " << p.recurrence() << std::endl;
            std::cout << "Player 1 won the game!" << std::endl;
            return 0;
        }
	}
}
*/

// codice di andre
#include "player.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <thread>

using std::cout;
using std::cin;
using std::endl;
using std::ifstream;
using std::string;

int main(int argc, char **argv) {

    try {

        Player p1(1);
        p1.init_board("board_0.txt");

        Player p2(2);

        p1.load_board("board_0.txt");
        p1.move();
        p1.store_board("board_1.txt");

        p2.load_board("board_1.txt");
        p2.move();
        p2.store_board("board_2.txt");

        int round = 2;

        while (/*p1.valid_move() && p2.valid_move() && */ !p1.wins() && !p2.wins()) {

            p1.load_board("board_" + std::to_string(round) + ".txt");
            p1.move();
            p1.store_board("board_" + std::to_string(++round) + ".txt");

            p2.load_board("board_" + std::to_string(round) + ".txt");
            p2.move();
            p2.store_board("board_" + std::to_string(++round) + ".txt");

            std::cout << "round: " << round << std::endl;

        }

        (p1.wins()) ? cout << "Player 1 wins!" << endl : cout << "Player 2 wins!" << endl;

        p2.store_board("board_exit.txt", 0);

    }
    catch (player_exception &e) {

        std::cout << e.msg << std::endl;

    }
}
