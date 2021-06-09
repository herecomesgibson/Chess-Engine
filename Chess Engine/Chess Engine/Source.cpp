

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
//for counting the number of captures in the move tree with a specific depth, used for debugging
U64 perft_c(Chess::Board b, int depth) {
	MoveIter iter(b);
	U64 nodes = 0;
	U64 captures = 0;

	if (depth == 0) { return 0ULL; }

	for (Move move : iter) {
		Move_type type = move.get_move_type();

		b.enact(move);
		if (type == Capture) { captures++; }
		captures += perft_c(b, depth - 1);
		b.undo(move);
	}

	return captures;
}

int main()
{
	
	Chess::Board board = Chess::Board::Board_init();

	int depth = 4;

	int perftest = perft(board, depth);

	std::cout << "perftest depth: " << depth << "  nodes:   " << perftest << "\n";

	Chess::Board capboard = Chess::Board::Board_init();

	int caperftest = perft_c(capboard, depth);

	std::cout << "captures at perftest depth: " << depth << "  nodes:   " << caperftest << "\n";

	
	
	Chess::Board clean = Chess::Board::Board_init();
	

	
	//some pre built initial moves for debugging
	
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
	
	Move h3 = Move(Square(48), Square(40));

	Move fcap = Move(Square(5), Square(40), Move_type(1));
	clean.enact(oe4);
	clean.enact(td5);
	clean.enact(exd5);

	Move movelstt[256] = {0};

	clean.show();


	/*
	for (int g = 0; g < 256; g++) {
		if (movelstt[g].get_move_int() != 0) {
			clean.enact(movelstt[g]);
			clean.show();
			std::cout << "from: " << movelstt[g].from() << "  to:  " << movelstt[g].to() << "  move flag:   " << movelstt[g].get_move_type() << "\n";
			clean.undo(movelstt[g]);
			//std::cout << movelstt[g].get_move_int() << "\n";
		}
	}




	
	Piece pcc = clean.get_piece_t(Square(12));
	std::cout << "square 12 piece type:  " << pcc << "\n";

	Piece pccc = clean.get_piece_t(Square(28));
	std::cout << "square 28 piece type:  " << pccc << "\n";
	*/



	//Move movelst[218] = {};
	//useful print statements for debugging
	//std::cout << "color turn:" << board.get_turn() << "\n";
	//std::cout << "moveint:  " << movelst[0].get_move_int() << "\n";
	//std::cout << "movelst size: " << movelst->size() << "\n";
	//show_bitboard(surrounding(Square(63)));

	/* code for testing queen moves
	Chess::Board bo = Chess::Board::Board_init( true );

	U64 qmovs = bo.generate_legal_Qmoves(movelst, Square(0));
	show_bitboard(qmovs);
	*/


	//MoveIter list(board);

	/*
	print non 0 moves in movelst
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