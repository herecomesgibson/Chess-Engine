

#include <iostream>
#include <bitset>
#include "Board.h"
#include "Types.h"
#include "engine.h"


U64 perft(Chess::Board b, int depth) {
	MoveIter iter(b);
	U64 nodes = 0;
	if (depth == 1) { return (U64)iter.size(); }
	if (depth == 0) { return 1ULL; }

	for (Move move : iter) {
		b.enact(move);
		nodes += perft(b, depth - 1);
		b.undo(move);
	}
	return nodes;
}

int main()
{
	Chess::Board board = Chess::Board::Board_init();

	int depth = 5;
	int perftest = perft(board, depth);
	std::cout << "perftest depth: " << depth << "  nodes:   " << perftest << "\n";

	show_bitboard(surrounding(Square(63)));

	Square e2 = Square(12);
	Square e4 = Square(28);
	Move oe4 = Move(e2, e4);

	Square d7 = Square(51);
	Square d5 = Square(35);
	Move td5 = Move(d7, d5);

	Square a2 = Square(8);
	Square a4 = Square(24);
	Move a2a4 = Move(a2, a4);

	Move_type captype = Capture; 

	Move exd5 = Move(e4, d5, captype);

	//std::cout << "color:" << board.get_turn() << "\n";

	//board.show();

	Move movelst[218] = {}; //= new Move();
	//std::list<Move> movelst;



	
	/*
	std::cout << "moveint:  " << movelst[0].get_move_int() << "\n";
	board.enact(movelst[1]);
	board.show();

	
	//movelst = {};
	board.generate_legal_moves(movelst);
	//std::cout << "movelst size: " << movelst->size() << "\n";
	board.enact(movelst[3]);
	board.show();
	std::cout << "moveint:  " << movelst[0].get_move_int() << "\n";
	std::cout << "color turn:" << board.get_turn() << "\n";
	*/


	MoveIter list(board);


	/*
	board.enact(list.begin());
	for (int i = 0; i < 218; i++) {
		uint16_t temp = movelst[i].get_move_int();
		if (temp != 0) {
			std::cout << "move:" << i << "=" << temp << "\n";
		}

	}
	show_bitboard(Bish_moves[2][3]);
	show_bitboard(Bish_blockers[2][3]);
	
	show_bitboard(Bish_all(Square(58)));
	
	std::cout << "AAAAAAHHH==\n";
	
	for (int i = 0; i < 7; i++) {
		show_bitboard(NW_SE_WHITEdiagonal[i]);
	}
	*/
}