

#include <iostream>
#include "Board.h"
#include "Types.h"
#include <bitset>



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
	board.enact(movelst[20]);
	board.show();

	std::cout << "color turn:" << board.get_turn() << "\n";



	


	for (int i = 0; i < 218; i++) {
		uint16_t temp = movelst[i].get_move_int();
		if (temp != 0) {
			std::cout << "move:" << i << "=" << temp << "\n";
		}

	}
	U64 test = single_bitboards[27];
	Square ranktest = Square(33);
	U64 testprnt = test << 1;
	std::cout << fileof(ranktest);


	/*
	U64 test = 8;
	show_bitboard(test);
	U64 test2 = Lshift(8, test, WHITE);
	show_bitboard(test2);
	*/
}