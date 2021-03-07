

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

	Square a2 = Square(8);
	Square a4 = Square(24);
	Move a2a4 = Move(a2, a4);

	std::cout << "color:" << board.get_turn() << "\n";

	board.show();

	Move movelst[218] = {}; //= new Move();

	


	board.enact(a2a4);
	board.generate_legal_moves(movelst);
	board.show();
	std::cout << "moveint:  " << movelst[0].get_move_int() << "\n";
	board.enact(movelst[0]);
	board.show();

	std::cout << "color turn:" << board.get_turn() << "\n";


	int indexx = 3;
	


	for (int i = 0; i < 218; i++) {
		uint16_t temp = movelst[i].get_move_int();
		if (temp != 0) {
			std::cout << "move:" << i << "=" << temp << "\n";
		}

	}




	show_bitboard(Bish_moves[2][3]);
	show_bitboard(Bish_blockers[2][3]);
	
	std::cout << "==\n";

}