//encode all neccessary info to make generate_moves operate on a "single bit string" array of fixed ints

/*
Bitboard array index table of contents

0 - white pawns
1 - white knights
2 - white bishops
3 - white rooks
4 - white queens
5 - white king
 
6 - black pawns
7 - black knights
8 - black bishops
9 - black rooks
10 - black queens
11 - black king

*/
#pragma once
#define Board_HPP_


#include <bitset>
#include <iostream>
#include <array>
#include <vector>
#include <intrin.h>
#include "Types.h"
#include "magic.h"



const size_t NSQUARES = 64;

struct CastlingRights {
	bool whiteL = true;
	bool whiteR = true;

	bool blackL = true;
	bool blackR = true;

};

struct History {
	
	CastlingRights CR;

	Piece capture;

	//might use this to remove the move argument from the undo function.
	uint16_t move;

	constexpr History() : capture(no_piece), move(0) {}
};

namespace Chess {

	class Board {
		
		private:

			//Bitboard gamestate representation array
			U64 bitboards[14];

			//mailbox board representation array
			Piece board_mb[NSQUARES];

			//Variable for whose turn it is
			Color whose_turn;

			//number of completed plys(half moves)
			int plys;

			bool check;


		public: 
			
			History hist[256];

			//Board constructor, creates an empty position (no pieces, empty board)
			Board() : bitboards{ 0 }, board_mb{  }, whose_turn(WHITE), plys(0) {
				for (int i = 0; i < 64; i++) {
					board_mb[i] = no_piece;
				}
			}
			
			static Board Board_init(bool empty = false);//static so it can be called without an object existing
			
			inline Color get_turn() {
				return whose_turn;
			}
			
			inline Piece get_piece_t(Square sq) {
				Piece pc = board_mb[sq];
				return pc;
			}

			void show(void);//method to display current board set-up
			
			inline void place(Piece piece, Square square) {
				board_mb[square] = piece;
				bitboards[piece] |= single_bitboards[square];
			}//adds specified piece to specified square

			inline void remove(Square square) {
				bitboards[board_mb[square]] &= ~single_bitboards[square];
				board_mb[square] = no_piece;
			}//deletes piece from square
			inline void move_piece(Square from, Square to) {
				Piece pc = get_piece_t(from);
				remove(from);
				place(pc, to);
			}//impliments quite move
			inline void capture(Square from, Square to) {
				hist[plys].capture = board_mb[to];
				remove(to);
				move_piece(from, to);
			}

			void enact(Move move) {

				if (move.get_move_int() == 0) {
					std::cout << "enact called with move value 0" << "\n";
					return;
				}
				Move_type mvtype = move.get_move_type();
				switch (mvtype) {
				case Quiet:
					move_piece(move.from(), move.to());
					break;
				case Capture:
					capture(move.from(), move.to());
					break;
				}
				hist[plys].move = move.get_move_int();
				whose_turn = Color(whose_turn ^ BLACK);
				plys++;
			}

			void undo(Move move) {
				
				--plys;
				whose_turn = Color(whose_turn ^ BLACK);
				if (move.get_move_int() == 0) {
					std::cout << "undo called with move value 0" << "\n";
					return;
				}
				Move_type mvtype = move.get_move_type();

				switch (mvtype) {
				case Quiet:
					move_piece(move.to(), move.from());
					break;
				case Capture:
					move_piece(move.to(), move.from());
					place(hist[plys].capture, move.to());
					break;
				}

			}

			inline Color toplay() const { return whose_turn; }// fetch the color whose turn it is
			inline U64 get_bitboard(Piece piece) const { return bitboards[piece]; }//get bitboard for a specific piece
			inline int get_plys() const { return plys; }//get Num of moves played

			U64 us_all(Color us) {
				return us == WHITE ?
				bitboards[0] | bitboards[1] | bitboards[2] | bitboards[3] | bitboards[4] | bitboards[5] : 
				bitboards[6] | bitboards[7] | bitboards[8] | bitboards[9] | bitboards[10] | bitboards[11];
			}
			U64 them_all(Color us) {
				return us == BLACK?
					bitboards[0] | bitboards[1] | bitboards[2] | bitboards[3] | bitboards[4] | bitboards[5] :
					bitboards[6] | bitboards[7] | bitboards[8] | bitboards[9] | bitboards[10] | bitboards[11];
			}

			

			U64* encode_board(Board board);
			Board read_encoding(U64* bblst);



			//adds bishop moves from a given square to the movelst
			Move* Bishop(Move* movelst, Color us, Square s) {

				int idxadj = (us == WHITE) ? 0 : 6;
				U64 usall = us_all(us);
				U64 themall = them_all(us);
				U64 allp = usall | themall;

				U64 bmask = Bish_bm[s];
				int numbits = popcount(bmask);
				U64 bmovebb = Bish_magic_moves[s][((bmask & allp) * Bish_magics[s]) >> (64 - numbits)] & (~usall);

				while (bmovebb) {
					Square to = pop_lsb(&bmovebb);
					if (single_bitboards[to] & themall) {
						*movelst++ = Move(s, to, Move_type(1));
					}
					else {
						*movelst++ = Move(s, to);
					}
				}

				return movelst;
			}

			//for returning an attack bitboard from a given square, used while looking for checks
			U64 aBishop(Color us, Square s) {
				int idxadj = (us == WHITE) ? 0 : 6;
				U64 usall = us_all(us);
				U64 themall = them_all(us);
				U64 allp = usall | themall;

				U64 bmask = Bish_bm[s];
				int numbits = popcount(bmask);
				U64 bmovebb = Bish_magic_moves[s][((bmask & allp) * Bish_magics[s]) >> (64 - numbits)] & (~usall);

				return bmovebb;
			}

			Move* Rook(Move* movelst, Color us, Square s) {

				int idxadj = (us == WHITE) ? 0 : 6;

				U64 usall = us_all(us);
				U64 themall = them_all(us);
				U64 allp = usall | themall;


				U64 rmask = Rook_bm[s];
				int numbits = popcount(rmask);
				U64 rmovebb = Rook_magic_moves[s][((rmask & allp) * Rook_magics[s]) >> (64 - numbits)] & (~usall);
				while (rmovebb) {
					Square to = pop_lsb(&rmovebb);
					if (single_bitboards[to] & themall) {
						*movelst++ = Move(s, to, Move_type(1));
					}
					else {
						*movelst++ = Move(s, to);
					}
				}
				return movelst;
			}

			//function for getting a bitboard of possible rook moves for a given square, for check checking
			U64 aRook(Color us, Square s) {
				int idxadj = (us == WHITE) ? 0 : 6;

				U64 usall = us_all(us);
				U64 themall = them_all(us);
				U64 allp = usall | themall;

				U64 rmask = Rook_bm[s];
				int numbits = popcount(rmask);
				U64 rmovebb = Rook_magic_moves[s][((rmask & allp) * Rook_magics[s]) >> (64 - numbits)] & (~usall);

				return rmovebb;
			}


			Move* Queen(Move* movelst, Color us, Square s) {

				int idxadj = (us == WHITE) ? 0 : 6;

				U64 usall = us_all(us);
				U64 themall = them_all(us);
				U64 allp = usall | themall;

				U64 rmask = Rook_bm[s];
				U64 bmask = Bish_bm[s];
				int rnumbits = popcount(rmask);
				int bnumbits = popcount(bmask);
				U64 movebb = (Rook_magic_moves[s][((rmask & allp) * Rook_magics[s]) >> (64 - rnumbits)] & (~usall)) | (Bish_magic_moves[s][((bmask & allp) * Bish_magics[s]) >> (64 - bnumbits)] & (~usall));
				while (movebb) {
					Square to = pop_lsb(&movebb);
					if (single_bitboards[to] & themall) {
						*movelst++ = Move(s, to, Move_type(1));
					}
					else {
						*movelst++ = Move(s, to);
					}
				}


				return movelst;
			}
			//returns bitboard of valid queen moves from a given square
			U64 aQueen(Color us, Square s) {
				int idxadj = (us == WHITE) ? 0 : 6;

				U64 usall = us_all(us);
				U64 themall = them_all(us);
				U64 allp = usall | themall;

				U64 rmask = Rook_bm[s];
				U64 bmask = Bish_bm[s];
				int rnumbits = popcount(rmask);
				int bnumbits = popcount(bmask);
				U64 movebb = (Rook_magic_moves[s][((rmask & allp) * Rook_magics[s]) >> (64 - rnumbits)] & (~usall)) | (Bish_magic_moves[s][((bmask & allp) * Bish_magics[s]) >> (64 - bnumbits)] & (~usall));

				return movebb;
			}

			Move* Knight(Move* movelst, Color us, Square s) {

				int idxadj = (us == WHITE) ? 0 : 6;
				U64 usall = us_all(us);
				U64 themall = them_all(us);
				U64 allp = usall | themall;

				U64 moves = knight_moves[s];
				while (moves) {
					Square dest = pop_lsb(&moves);
					bool uscol = single_bitboards[dest] & usall;//bool for not self capturing
					if (uscol == 0) {
						bool capturecol = single_bitboards[dest] & themall;//bool to determine whether its a capture or a quiet move
						if (capturecol) {
							*movelst++ = Move(s, dest, Move_type(1));
						}
						else {
							*movelst++ = Move(s, dest);
						}
					}
				}

				return movelst;
			}
			//returns bitboard of knight moves
			U64 aKnight( Color us, Square s) {
				U64 moves = knight_moves[s];
				return moves;
			}

			Move* Pawn(Move* movelst, Color Us) {
				
				int idxadj = (Us == WHITE) ? 0 : 6;

				U64 usall = us_all(Us);
				U64 themall = them_all(Us);
				U64 allp = usall | themall;
				
				U64 us_pawns = bitboards[idxadj];
				while (us_pawns) {
					Square s = pop_lsb(&us_pawns);

					U64 potential_moves = shift<NORTH>(single_bitboards[s], Us);//for quiet single pushes
					U64 potential_moved = Lshift(16, single_bitboards[s], Us);//// for quite double pushes
					U64 left_attack = Lshift(7, single_bitboards[s], Us);//forward-left pawn attacks
					U64 right_attack = Lshift(9, single_bitboards[s], Us);//forward-right pawn attacks

					if (~(potential_moves & allp)) {//single pushes
						*movelst++ = Move(s, Square(lsb(potential_moves)));
					}
					if (~(potential_moved & allp) && pawn_clean(Us, Square(s))) {//double pushes
						*movelst++ = Move(s, Square(lsb(potential_moved)));
					}
					if ((left_attack & themall) && (abs(fileof(s) - fileof(Square(lsb(left_attack)))) <= 1)) {//first checks that there is actually a piece to attack then double checks the shift didnt wrap around from a to h file, 
						*movelst++ = Move(s, Square(lsb(left_attack)), Move_type(1));
					}
					if ((right_attack & themall) && (abs(fileof(s) - fileof(Square(lsb(right_attack)))) <= 1)) {//first checks that there is actually a piece to attack then double checks the shift didnt wrap around from a to h file, 
						*movelst++ = Move(s, Square(lsb(right_attack)), Move_type(1));
					}
				}
				return movelst;
			}

			//for pawn attacks (they need a seperate function since their attack patterns are distinct from their movement patterns, and we need exlusively the attack patterns while looking for checks)
			U64 aPawn( Color Us, Square s) {

				int idxadj = (Us == WHITE) ? 0 : 6;

				U64 usall = us_all(Us);
				U64 themall = them_all(Us);
				U64 allp = usall | themall;

				if (Us == WHITE) {
					U64 pmov = ((single_bitboards[s] << 7) | (single_bitboards[s] << 9)) & themall;
					return pmov;
				}
				else if (Us == BLACK) {
					U64 pmov = ((single_bitboards[s] >> 7) | (single_bitboards[s] >> 9)) & themall;
					return pmov;
				}
			}

			//for quiet pawn moves
			Move* qPawn(Move* movelst, Color Us, Square s) {

				int idxadj = (Us == WHITE) ? 0 : 6;

				U64 usall = us_all(Us);
				U64 themall = them_all(Us);
				U64 allp = usall | themall;

				if (Us == WHITE) {
					if (rankof(s) == 6) { // promotions, todo
						;
					}
					U64 pmov = (single_bitboards[s] << 8);//signle push
					if (rankof(s) == 1) {//potential initial double push

					}

				}


				return movelst;
			}

			Move* King(Move* movelst, Color Us, Square sq) {

				int idxadj = (Us == WHITE) ? 0 : 6;

				U64 usall = us_all(Us);
				U64 themall = them_all(Us);
				U64 allp = usall | themall;

				U64 kmoves = surrounding(sq) & ~usall;
				while (kmoves) {
					Square s = pop_lsb(&kmoves);
					if (popcount(attacksTo(Us, s)) != 0) { // if the potential move square is attacked don't add it to the list
						continue;
					}
					if ((single_bitboards[s] & themall)) {
						*movelst++ = Move(sq, s, Move_type(1));
					}
					else {
						*movelst++ = Move(sq, s);
					}
				}

				return movelst;
			}

			//returns a bitboard of possible king moves, excluding moves that leave us in check
			U64 aKing( Color Us, Square sq) {

				int idxadj = (Us == WHITE) ? 0 : 6;

				U64 usall = us_all(Us);
				U64 themall = them_all(Us);
				U64 allp = usall | themall;
				U64 retboard = 0;

				U64 kmoves = surrounding(sq) & ~usall;
				while (kmoves) {
					Square s = pop_lsb(&kmoves);
					if (popcount(attacksTo(Us, s)) != 0) { // if the potential move square is attacked don't add it to the list
						continue;
					}
					retboard |= single_bitboards[s];
				}
				return retboard;
			}

			//returns a bitboard of all enemy pieces currently attacking a square
			U64 attacksTo(Color Us, Square sq) {



				int idxadj = (Us == 1) ? idxadj = 6 : idxadj = 0; 
				int them_idxadj = (Us == 0) ? idxadj = 6 : idxadj = 0;

				U64 pawnatt = aPawn(Us, sq) & (bitboards[them_idxadj]);
				U64 knightatt = aKnight(Us, sq) & (bitboards[1 + them_idxadj]);
				U64 Bishatt = aBishop(Us, sq) & (bitboards[2 + them_idxadj]);
				U64 Rookatt = aRook(Us, sq) & (bitboards[3 + them_idxadj]);
				U64 Queenatt = aQueen(Us, sq) & (bitboards[4 + them_idxadj]);

				return (pawnatt | knightatt | Bishatt | Rookatt | Queenatt);
			}





			Move* generate_legal_moves(Move* movelst) {
				 
				//temp
				int temp = 0;

				Color Us = whose_turn;
				Color Them = Color(Us ^ BLACK);
				
				int idxadj = 0;
				if (Us == BLACK) {
					idxadj = 6;
				}


				//int idxadj = (Us == BLACK ) ? idxadj = 6 : idxadj = 0;    // for accessing specific pieces' bitboards (idxadj = index adjustment)

				int them_idxadj = (Us == 0) ? them_idxadj = 6 : them_idxadj = 0;

				U64 usall = us_all(Us);
				U64 themall = them_all(Us);
				U64 allp = usall | themall;
				U64 checkers = 0;//bitboard that gets populated with all pieces checking the king

				//checking for checks


				U64 us_king = bitboards[idxadj+5];

				if (popcount(us_king) == 1) {
					Square kingsq = lsb(us_king);
					checkers = attacksTo(Us, kingsq);
				}
				else {
					throw std::invalid_argument(" There is more or less than one king for one of the colors");
				}

				
				// CHECKS
				if (popcount(checkers) > 1) { // in the case of a double check we have to move the king
					
					U64 evasions = aKing(Us, lsb(us_king));
					while (evasions) {
						Square s = pop_lsb(&evasions);
						if (single_bitboards[s] & themall) {
							*movelst++ = Move(lsb(us_king), s, Move_type(1));
						}
						else {
							*movelst++ = Move(lsb(us_king), s);
						}
					}
					
					
					return movelst;//return early since moving the king is our only option

				}
				

				else if (popcount(checkers) == 1) { // in the case of single checks
					
					//OPTIONS
					//1. move king

					U64 evasions = aKing(Us, lsb(us_king));
					while (evasions) {
						Square s = pop_lsb(&evasions);
						if (single_bitboards[s] & themall) {
							*movelst++ = Move(lsb(us_king), s, Move_type(1));
						}
						else {
							*movelst++ = Move(lsb(us_king), s);
						}
					}

					//2. capture checking piece

					//get bitboard of all our pieces that are attacking the checker
					U64 our_saviors = attacksTo(Them, lsb(checkers));

					while (our_saviors) {
						Square loc = pop_lsb(&our_saviors);
						if (loc != lsb(us_king)) {//king moves where already added, also the king may not be able to capture the attacker if it is defended
							*movelst++ = Move(loc, lsb(checkers), Move_type(1));
						}
							
					}
					
					//3. block check (create pin, not for knight checks)

					U64 checkers_pop = checkers;

					while (checkers_pop) {
						Square loc = pop_lsb(&checkers_pop);
						Piece checker_type = board_mb[loc];

					}


					return movelst;

					
				}
				
				//King moves

				U64 kmoves = surrounding(lsb(us_king)) & ~usall;
				while (kmoves) {
					Square s = pop_lsb(&kmoves);
					if (single_bitboards[s] & themall) {
						*movelst++ = Move(lsb(us_king), s, Move_type(1));
					}
					else {
						*movelst++ = Move(lsb(us_king), s);
					}
				}
				
				
				//Bishop moves
				U64 us_bishops = bitboards[idxadj + 2];
				while (us_bishops) {
					Square s = pop_lsb(&us_bishops);
					movelst = Bishop(movelst, Us, s);
				}
				
				//Rook moves
				U64 us_rooks = bitboards[idxadj + 3];
				while (us_rooks) {
					Square s = pop_lsb(&us_rooks);
					movelst = Rook(movelst, Us, s);
				}
				
				
				//Queen moves
				U64 oQueen = bitboards[idxadj + 4];
				while (oQueen) {
					Square s = pop_lsb(&oQueen);
					movelst = Queen(movelst, Us, s);
				}
				
				//Knight Moves (lol)
				U64 us_knights = bitboards[idxadj + 1];
				while (us_knights) {
					Square s = pop_lsb(&us_knights);
					movelst = Knight(movelst, Us, s);
				}

				//pawn moves
				movelst = Pawn(movelst, Us);

				return movelst;
			}


	};//Board class

	//initialize board, empty is default set to false, if true the board will be filled with pieces of type no_piece.
	Board Board::Board_init(bool empty) {
		magic_rook_init();
		magic_bish_init();
		engine_init();
		Board new_board;
		
		new_board.check = false;
		new_board.whose_turn = WHITE;
		new_board.plys = 0;

		if (empty) {
			for (int i = 0; i < 64; i++) {
				new_board.board_mb[i] = no_piece;
			}
			return new_board;
		}

		new_board.board_mb[0] = WHITE_rook;
		new_board.board_mb[1] = WHITE_knight;
		new_board.board_mb[2] = WHITE_bishop;
		new_board.board_mb[3] = WHITE_queen;
		new_board.board_mb[4] = WHITE_king;
		new_board.board_mb[5] = WHITE_bishop;
		new_board.board_mb[6] = WHITE_knight;
		new_board.board_mb[7] = WHITE_rook;

		new_board.board_mb[8] = WHITE_pawn;
		new_board.board_mb[9] = WHITE_pawn;
		new_board.board_mb[10] = WHITE_pawn;
		new_board.board_mb[11] = WHITE_pawn;
		new_board.board_mb[12] = WHITE_pawn;
		new_board.board_mb[13] = WHITE_pawn;
		new_board.board_mb[14] = WHITE_pawn;
		new_board.board_mb[15] = WHITE_pawn;

		new_board.board_mb[63] = BLACK_rook;
		new_board.board_mb[62] = BLACK_knight;
		new_board.board_mb[61] = BLACK_bishop;
		new_board.board_mb[60] = BLACK_king;
		new_board.board_mb[59] = BLACK_queen;
		new_board.board_mb[58] = BLACK_bishop;
		new_board.board_mb[57] = BLACK_knight;
		new_board.board_mb[56] = BLACK_rook;

		new_board.board_mb[55] = BLACK_pawn;
		new_board.board_mb[54] = BLACK_pawn;
		new_board.board_mb[53] = BLACK_pawn;
		new_board.board_mb[52] = BLACK_pawn;
		new_board.board_mb[51] = BLACK_pawn;
		new_board.board_mb[50] = BLACK_pawn;
		new_board.board_mb[49] = BLACK_pawn;
		new_board.board_mb[48] = BLACK_pawn;

		for (int i = 16; i < 48; i++) {
			new_board.board_mb[i] = no_piece;
		}

		new_board.bitboards[0] = 0xff00;
		new_board.bitboards[1] = 0x42;
		new_board.bitboards[2] = 0x24;
		new_board.bitboards[3] = 0x81;
		new_board.bitboards[4] = 0x8;
		new_board.bitboards[5] = 0x10;

		new_board.bitboards[6] = 0xff000000000000;
		new_board.bitboards[7] = 0x4200000000000000;
		new_board.bitboards[8] = 0x2400000000000000;
		new_board.bitboards[9] = 0x8100000000000000;
		new_board.bitboards[10] = 0x800000000000000;
		new_board.bitboards[11] = 0x1000000000000000;


		return new_board;

	}

	void Board::show(void) {
		std::vector<std::string> board_rep(NSQUARES);
		std::bitset<64>wp(bitboards[0]);
		std::bitset<64>wn(bitboards[1]);
		std::bitset<64>wb(bitboards[2]);
		std::bitset<64>wr(bitboards[3]);
		std::bitset<64>wq(bitboards[4]);
		std::bitset<64>wk(bitboards[5]);

		std::bitset<64>bp(bitboards[6]);
		std::bitset<64>bn(bitboards[7]);
		std::bitset<64>bb(bitboards[8]);
		std::bitset<64>br(bitboards[9]);
		std::bitset<64>bq(bitboards[10]);
		std::bitset<64>bk(bitboards[11]);

		for (int g = 0; g < 64; g++) {
			if (wk[g] == 1) { board_rep[g] = "K"; }
			else if (wp[g] == 1) { board_rep[g] = "P"; }
			else if (wr[g] == 1) { board_rep[g] = "R"; }
			else if (wn[g] == 1) { board_rep[g] = "N"; }
			else if (wb[g] == 1) { board_rep[g] = "B"; }
			else if (wq[g] == 1) { board_rep[g] = "Q"; }
			else if (bk[g] == 1) { board_rep[g] = "K"; }
			else if (bp[g] == 1) { board_rep[g] = "P"; }
			else if (br[g] == 1) { board_rep[g] = "R"; }
			else if (bn[g] == 1) { board_rep[g] = "N"; }
			else if (bb[g] == 1) { board_rep[g] = "B"; }
			else if (bq[g] == 1) { board_rep[g] = "Q"; }
			else { board_rep[g] = "_"; }
		}
		std::cout << "--------" << "\n";
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				int index = (56 - (i * 8)) + j;
				std::cout << board_rep[index] << " ";
			}
			std::cout << "\n";
		}
		std::cout << "--------" << "\n";
	}
}//namespace





