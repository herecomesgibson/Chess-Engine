#pragma once

//#include "Board.h"
#include <assert.h>

//define U64 to be a 64 bit unsigned int for use as bitboards
#ifndef _U64
typedef std::uint64_t U64;
#define _U64
#endif  _U64


constexpr U64 FileA = 0x0101010101010101ULL;
constexpr U64 FileB = FileA << 1;
constexpr U64 FileC = FileA << 2;
constexpr U64 FileD = FileA << 3;
constexpr U64 FileE = FileA << 4;
constexpr U64 FileF = FileA << 5;
constexpr U64 FileG = FileA << 6;
constexpr U64 FileH = FileA << 7;

constexpr U64 Rank1 = 0xFF;
constexpr U64 Rank2 = Rank1 << (8 * 1);
constexpr U64 Rank3 = Rank1 << (8 * 2);
constexpr U64 Rank4 = Rank1 << (8 * 3);
constexpr U64 Rank5 = Rank1 << (8 * 4);
constexpr U64 Rank6 = Rank1 << (8 * 5);
constexpr U64 Rank7 = Rank1 << (8 * 6);
constexpr U64 Rank8 = Rank1 << (8 * 7);

U64 knight_moves[64] = {};

const U64 Ranks[8] = {
	Rank1, Rank2, Rank3, Rank4, Rank5, Rank6, Rank7, Rank8, 
};

const U64 Files[8] = {
	FileA, FileB, FileC, FileD, FileE, FileF, FileG, FileH
};


const U64 NE_SW_BLACKdiagnal[7] = {
	0x8040,
	0x80402010,
	0x804020100804,
	0x8040201008040201,
	0x2010080402010000,
	0x804020100000000,
	0x201000000000000,
};

const U64 single_bitboards[64] = {
	0x1, 0x2, 0x4, 0x8,
	0x10, 0x20, 0x40, 0x80,
	0x100, 0x200, 0x400, 0x800,
	0x1000, 0x2000, 0x4000, 0x8000,
	0x10000, 0x20000, 0x40000, 0x80000,
	0x100000, 0x200000, 0x400000, 0x800000,
	0x1000000, 0x2000000, 0x4000000, 0x8000000,
	0x10000000, 0x20000000, 0x40000000, 0x80000000,
	0x100000000, 0x200000000, 0x400000000, 0x800000000,
	0x1000000000, 0x2000000000, 0x4000000000, 0x8000000000,
	0x10000000000, 0x20000000000, 0x40000000000, 0x80000000000,
	0x100000000000, 0x200000000000, 0x400000000000, 0x800000000000,
	0x1000000000000, 0x2000000000000, 0x4000000000000, 0x8000000000000,
	0x10000000000000, 0x20000000000000, 0x40000000000000, 0x80000000000000,
	0x100000000000000, 0x200000000000000, 0x400000000000000, 0x800000000000000,
	0x1000000000000000, 0x2000000000000000, 0x4000000000000000, 0x8000000000000000,
};


//direction enum taken from stockfish https://github.com/official-stockfish/Stockfish/tree/master/src
enum Direction : int {
	NORTH = 8,
	EAST = 1,
	SOUTH = -NORTH,
	WEST = -EAST,

	NORTH_EAST = NORTH + EAST,
	SOUTH_EAST = SOUTH + EAST,
	SOUTH_WEST = SOUTH + WEST,
	NORTH_WEST = NORTH + WEST
};

enum Color : int {
	WHITE,
	BLACK = 1
};

enum Piece_type : int {
	pawn,
	knight,
	bishop,
	rook,
	queen,
	king,
};

enum Piece : int {
	WHITE_pawn=0,
	WHITE_knight=1,
	WHITE_bishop=2,
	WHITE_rook,
	WHITE_queen,
	WHITE_king,

	BLACK_pawn,
	BLACK_knight,
	BLACK_bishop,
	BLACK_rook,
	BLACK_queen,
	BLACK_king,

	no_piece
};

enum Square : int {
	a1, b1, c1, d1, e1, f1, g1, h1,
	a2, b2, c2, d2, e2, f2, g2, h2,
	a3, b3, c3, d3, e3, f3, g3, h3,
	a4, b4, c4, d4, e4, f4, g4, h4,
	a5, b5, c5, d5, e5, f5, g5, h5,
	a6, b6, c6, d6, e6, f6, g6, h6,
	a7, b7, c7, d7, e7, f7, g7, h7,
	a8, b8, c8, d8, e8, f8, g8, h8,

};

enum Move_type : int {

	Quiet,
	Capture,
	Evade,
	Castle_k_WHITE,
	Castle_q_WHITE,
	Castle_k_BLACK,
	Castle_q_BLACK,
	en_pessant,

};
/*
Moves are stored as 16 bit integers.
bits 0-6: store the destination square for the move
bits 7-12: store the originating square
bits 13-15: store flags for the move

*/
class Move {
	private:

		uint16_t move;

	public:
		//mailbox

		inline Move() : move(0) {}

		inline Move(Square from, Square to) : move(0) {
			move = ((uint16_t)from << 6) | (uint16_t)to;
		}

		inline Move(Square from, Square to, Move_type flag) : move(0) {
			move = flag << 12 | ((uint16_t)from << 6) | (uint16_t)to;
		}

		template<Move_type Mt = Quiet> inline Move Move_make(Square f, Square t, Piece pc) {

		}
		template<> inline Move Move_make<Capture>(Square f, Square t, Piece pc) {

		}
		inline Square from() { return Square((move >> 6) & 0x3f); }
		inline Square to() { return Square(move & 0x3f); }
		inline Move_type get_move_type() { return Move_type((move >> 12) & 0xf); }
		inline uint16_t get_move_int() { 
			if (move) {
				return move;
			}
			else {
				return 0;
			}
			
		}
		inline void print() {
			std::cout << move << "\n";
		}

};

class Capture : public Move {
	public:

	Piece captured_piece;


};

const int DB64[64] = {
	0, 47,  1, 56, 48, 27,  2, 60,
   57, 49, 41, 37, 28, 16,  3, 61,
   54, 58, 35, 52, 50, 42, 21, 44,
   38, 32, 29, 23, 17, 11,  4, 62,
   46, 55, 26, 59, 40, 36, 15, 53,
   34, 51, 20, 43, 31, 22, 10, 45,
   25, 39, 14, 33, 19, 30,  9, 24,
   13, 18,  8, 12,  7,  6,  5, 63
};

const U64 DBmagic = 0x03f79d71b4cb0a89;

inline Square lsb(U64 bb) {
	return Square(DB64[DBmagic * (bb ^ (bb - 1)) >> 58]);
}

inline Square pop_lsb(U64* bb) {
	int location = lsb(*bb);
	*bb &= (*bb - 1);
	return Square(location);
}

inline int rankof(Square s) {
	return (s >> 3);
}
constexpr int fileof(Square s) { 
	return (s & 0b111); 
}
inline int rankofint(int s) {
	return (s >> 3);
}

inline void show_bitboard(U64 bitboard) {
	//uint64_t board_representation = bitboards[0] | bitboards[1] | bitboards[2] | bitboards[3] | bitboards[4] | bitboards[5] | bitboards[6] | bitboards[7] | bitboards[8] | bitboards[9] | bitboards[10] | bitboards[11];
	std::bitset<64>bitset_board(bitboard);
	int index = 0;
	std::cout << "--------" << "\n";
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			index = (56 - (i * 8)) + j;
			std::cout << bitset_board[index] << " ";
		}
		std::cout << "\n";
	}
	std::cout << "--------";

}

template<Direction D>
 constexpr U64 shift(U64 bb, Color C) {
	switch (D) {
		case NORTH:
			return C == WHITE ? bb << 8 : bb >> 8;
			break;
		case NORTH_EAST:
			return C == WHITE ? bb << 9 : bb >> 9;
			break;
		case NORTH_WEST:
			return C == WHITE ? bb << 7 : bb >> 7;
			break;
		case EAST:
			return C == WHITE ? (bb & ~FileH) >> 1 : (bb & ~FileA) << 1;
			break;
		case WEST:
			return C == WHITE ? (bb & ~FileA) << 1 : (bb & ~FileH) >> 1;
			break;
		case SOUTH:
			return C == WHITE ? bb >> 8 : bb << 8;
			break;
		case SOUTH_EAST:
			return C == WHITE ? (bb & ~FileH) >> 7 : (bb & ~FileA) << 7;
			break;
		case SOUTH_WEST:
			return C == WHITE ? (bb & ~FileA) >> 9 : (bb & ~FileH) << 9;
			break;
	}
}

/*
template<Color C>
constexpr U64 Cshift(U64 bb, Direction D) {
	return C == WHITE ? <D>shift(bb) : C == BLACK ? <-D>shift(bb) : 0;
}
*/

inline U64 Lshift(int shiftby, U64 bb, Color color) {
	switch (color) {
		case WHITE:
			return bb << shiftby;
			break;
		case BLACK:
			return bb >> shiftby;
			break;

	}
}

inline U64 Rshift(int shiftby, U64 bb, Color color) {
	switch (color) {
	case WHITE:
		return bb >> shiftby;
		break;
	case BLACK:
		return bb << shiftby;
		break;

	}
}

inline bool pawn_clean(Color Us, Square s) {
	switch (Us) {
	case WHITE:
		return (s < 16);
		break;
	case BLACK:
		return (s > 47);
		break;
	}
}

void engine_init() {
	for (int sq = 0; sq < 64; sq++) {
		for (int step : { 6, 10, 15, 17 }) {
			U64 move1 = single_bitboards[sq] << step;
			U64 move2 = single_bitboards[sq] >> step;
			if (std::abs(fileof(Square(lsb(move1))) - fileof(Square(sq))) <= 2 && (move1 != 0)) {//check if knight move is legal
				knight_moves[sq] |= move1;//add moves to bitboard
			}
			if (std::abs(fileof(Square(lsb(move2))) - fileof(Square(sq))) <= 2 && (move2 != 0)) {//check if knight move is legal
				knight_moves[sq] |= move2;//add moves to bitboard
			}

		}
	}
}
//     0b111
//magic bitboards items

U64 Rook_blockers[64][1024];

U64 Rook_moves[64][1024];

U64 Rook_all(Square sq) {
	
	U64 retboard = 0;
	U64 loc = single_bitboards[sq];
	U64 bit = loc;

	while (bit <<= 8) {
		retboard |= bit;
	}
	bit = loc;
	while (loc >>= 8) {
		retboard |= bit;
	}
	bit = loc << 1;
	return 0;
	
}

void magic_rook_init() {
	for (int sq = 0; sq < 64; sq++) {

		//rank possiblilties
		for (int i = 0; i < 6; i++) {

		}

	}
}