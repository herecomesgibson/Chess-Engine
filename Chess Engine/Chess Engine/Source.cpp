

#include <iostream>
#include <bitset>
#include "Board.h"
#include "Types.h"




int main()
{



	Chess::Board board = Chess::Board::Board_init();



	Square e2 = Square(12);
	Square e4 = Square(28);
	Move testpls = Move(e2, e4);

	Square e6 = Square(37);
	Square e8 = Square(53);
	Move testpls2 = Move(e8, e6);


	std::cout << "color:" << board.get_turn() << "\n";

	board.show();

	Move movelst[218] = {}; //= new Move();

	

	board.enact(testpls);
	board.enact(testpls2);
	board.generate_legal_moves(movelst);
	board.show();
	std::cout << movelst[0].get_move_int() << "\n";
	board.enact(movelst[0]);
	board.show();

	std::cout << "color turn:" << board.get_turn() << "\n";


	int indexx = 3;
	
	show_bitboard(Bish_blockers[2][indexx]);
	show_bitboard(Bish_moves[2][indexx]);

	for (int i = 0; i < 218; i++) {
		uint16_t temp = movelst[i].get_move_int();
		if (temp != 0) {
			std::cout << "move:" << i << "=" << temp << "\n";
		}

	}

	U64 test_blocker = 2560;
	show_bitboard(test_blocker);
	int magic_index = (test_blocker * Bish_magics[2]) >> (64 - 5);
	std::cout << "index" << magic_index << "\n";
	show_bitboard(Bish_moves[2][magic_index]);


	std::cout << "==\n";

}