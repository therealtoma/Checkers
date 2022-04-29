#include "player.hpp"

class Player{
    public:
        enum piece {x, o, X, O, e};

        Player(int player_nr) {
            if (player_nr != 0 && player_nr != 1)
                // trows exception
                throw player_exception{player_exception::index_out_of_bounds,
                                       "The player number can't be " << player_nr << "!\nIt must be 0 or 1!"};

            this->player_nr = player_nr; // sets the player number

            memory = new Impl; // initializes the memory
            memory->next = nullptr; // on the first call, the next pointer is nullptr
            memory->prev = nullptr; // on the first call, the prev pointer is nullptr
            // initializes the board
            memory->board = {
                    {e, x, e, x, e, x, e, x},
                    {x, e, x, e, x, e, x, e},
                    {e, x, e, x, e, x, e, x},
                    {e, e, e, e, e, e, e, e},
                    {e, e, e, e, e, e, e, e},
                    {o, e, o, e, o, e, o, e},
                    {e, o, e, o, e, o, e, o},
                    {o, e, o, e, o, e, o, e}
            };

            std::cout << "Player " << player_nr << " created!\n";
        }
            ~Player(){}
            Player(const Player&){}
            Player& operator=(const Player&){}
            piece operator()(int r, int c, int history_offset = 0) const{}
            void load_board(const string& filename){}
            void store_board(const string& filename, int history_offset = 0) const{}
            void init_board(const string& filename) const{}
            void move(){}
            bool valid_move() const{}
            void pop(){}
            bool wins(int player_nr) const{}
            bool wins() const{}
            bool loses(int player_nr) const{}
            bool loses() const{}
            int recurrence() const{}

    private:
        int player_nr;

        struct Impl{
            pImpl next;
            pImpl prev;

            piece board[8][8];
        };

    typedef Impl* pImpl;

    pImpl memory; // a pointer to the latest board
};
int main(){

    Player p1(0);
    std::cout << "end of the program" << std::endl;

    return 0;
}