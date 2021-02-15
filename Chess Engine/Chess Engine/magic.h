#pragma once


#include <iostream>
#include <fstream>
#include <random>
#include"Types.h"


constexpr U64 one = 1;

constexpr U64 board_edges = Rank1 | Rank8 | FileA | FileH;

U64 AFile_inside = FileA ^ (single_bitboards[0] | single_bitboards[56]);

U64 HFile_inside = FileH ^ (single_bitboards[7] | single_bitboards[63]);

U64 Rank1_inside = Rank1 ^ (single_bitboards[0] | single_bitboards[7]);

U64 Rank8_inside = Rank8 ^ (single_bitboards[56] | single_bitboards[63]);

const U64 NE_SW_BLACKdiagonal[7] = {
	0x8040,
	0x80402010,
	0x804020100804,
	0x8040201008040201,
	0x2010080402010000,
	0x804020100000000,
	0x201000000000000,
};
const U64 NW_SE_BLACKdiagonal[8] = {
	0x1,
	0x10204,
	0x102040810,
	0x1020408102040,
	0x204081020408000,
	0x810204080000000,
	0x2040800000000000,
	0x8000000000000000,
};
const U64 NE_SW_WHITEdiagonal[8] = {
	0x80,
	0x804020,
	0x8040201008,
	0x80402010080402,
	0x4020100804020100,
	0x1008040201000000,
	0x402010000000000,
	0x100000000000000,
};
const U64 NW_SE_WHITEdiagonal[7] = {
	0x102,
	0x1020408,
	0x10204081020,
	0x102040810204080,
	0x408102040800000,
	0x1020408000000000,
	0x4080000000000000,
};


U64 Rook_blockers[64][4096];
U64 Rook_moves[64][4096];
U64 Rook_magics[64];

U64 Bish_blockers[64][1024];
U64 Bish_moves[64][1024];
U64 Bish_magics[64];



//blocker mask of all potential rook moves - edge moves. If edges is set to false, the edge bits will be included in the output
U64 Rook_all(Square sq, bool edges = true) {
	U64 bedges = board_edges;
	U64 moves = rankofbb(sq) | fileofbb(sq);
	if (edges == true) {
		if (rankof(sq) == 0) { bedges ^= Rank1_inside; }
		if (rankof(sq) == 7) { bedges ^= Rank8_inside; }
		if (fileof(sq) == 0) { bedges ^= AFile_inside; }
		if (fileof(sq) == 7) { bedges ^= HFile_inside; }
		
		return moves ^ (single_bitboards[sq] | (bedges & moves));
	}
	else {
		return moves ^ single_bitboards[sq];
	}
}

U64 Bish_all(Square sq, bool edges = true) {

	U64 retboard = 0;

	bool foundone = false;
	bool foundtwo = false;
	U64 bit = single_bitboards[sq];

	for (int i = 0; i < 7; i++) {
		if (NE_SW_BLACKdiagonal[i] & bit) { retboard |= NE_SW_BLACKdiagonal[i]; }
		if (NW_SE_WHITEdiagonal[i] & bit) { retboard |= NW_SE_WHITEdiagonal[i]; }
	}
	for (int i = 0; i < 6; i++) {
		if (NW_SE_BLACKdiagonal[i] & bit) { retboard |= NW_SE_BLACKdiagonal[i]; }
		if (NE_SW_WHITEdiagonal[i] & bit) { retboard |= NE_SW_WHITEdiagonal[i]; }
	}
	if (!edges) {
		return retboard ^ single_bitboards[sq];
	}
	else {
		return retboard ^ (single_bitboards[sq] | (retboard & board_edges));
	}
}

//this function takes as input a swuare and a blockerboard and gives as output a moveboard all possible moves for a rook on the given square. NOTE the return moveboard here includes self captures
U64 Rook_movebb(Square sq, U64 blockerboard) {

	U64 retboard = 0;
	U64 possible_moves = Rook_all(sq, false);

	bool north = true;
	if (sq > 55) { north = false; }

	bool south = true;
	if (sq < 8) { south = false; }

	bool east = true;
	if (fileof(sq) == 7) { east = false; }

	bool west = true;
	if (fileof(sq) == 0) { west = false; }


	U64 bit = single_bitboards[sq];
	int square = sq;

	while (north) {
		square += 8;
		bit <<= 8;
		retboard |= bit;
		if ((bit & blockerboard) || (square > 55)) { north = false; }

	}

	bit = single_bitboards[sq];
	square = sq;

	while (south) {
		square -= 8;
		bit >>= 8;
		retboard |= bit;
		if ((bit & blockerboard) || (square < 8)) { south = false; }
	}

	bit = single_bitboards[sq];
	square = sq;

	while (east) {
		bit <<= 1;
		square += 1;
		retboard |= bit;
		if ((bit & blockerboard) || (fileof(Square(square)) == 7)) { east = false; }
	}

	bit = single_bitboards[sq];
	square = sq;

	while (west) {
		square -= 1;
		bit >>= 1;
		retboard |= bit;
		if ((bit & blockerboard) || (fileof(Square(square)) == 0)) { west = false; }
	}

	return retboard;
}
//given a square and a blockerboard thisfunction returns a valid moveboard for bishops on the square
U64 Bish_movebb(Square sq, U64 blockerboard) {
	U64 retboard = 0;

	bool northwest = true;
	if ((fileof(sq) == 0) || (rankof(sq) == 7)) { northwest = false; }

	bool northeast = true;
	if ((fileof(sq) == 7) || (rankof(sq) == 7)) { northeast = false; }

	bool southwest = true;
	if ((fileof(sq) == 0) || (rankof(sq) == 0)) { southwest = false; }

	bool southeast = true;
	if ((fileof(sq) == 7) || (rankof(sq) == 0)) { southeast = false; }


	U64 bit = single_bitboards[sq];
	int square = sq;

	while (northwest) {
		bit <<= 7;
		square += 7;
		retboard |= bit;
		if ((fileof(Square(square)) == 0) || (rankof(Square(square)) == 7) || (bit & blockerboard)) { northwest = false; }
	}

	bit = single_bitboards[sq];
	square = sq;

	while (northeast) {
		bit <<= 9;
		square += 9;
		retboard |= bit;
		if ((fileof(Square(square)) == 7) || (rankof(Square(square)) == 7) || (bit & blockerboard)) { northeast = false; }
	}

	bit = single_bitboards[sq];
	square = sq;

	while (southwest) {
		bit >>= 9;
		square -= 9;
		retboard |= bit;
		if ((fileof(Square(square)) == 0) || (rankof(Square(square)) == 0) || (bit & blockerboard)) { southwest = false; }
	}

	bit = single_bitboards[sq];
	square = sq;

	while (southeast) {
		bit >>= 7;
		square += 7;
		retboard |= bit;
		if ((fileof(Square(square)) == 7) || (rankof(Square(square)) == 0) || (bit & blockerboard)) { southeast = false; }
	}
	return retboard;
}

//given a blocker mask and an index (a number between 0 and 2^bits) return a unique blockerboard for the square
U64 generate_blocker(int index, U64 blockermask) {
	U64 blockerb = blockermask;
	int bitloc = 0;
	for (int i = 0; i < 64; i++) {
		if (blockermask & (one << i)) {//check each square in the blockermask for a piece
			if (!(index & (one << bitloc))) {// if the bit is unset in the index
				blockerb &= ~(one << i);
			}
			bitloc++;
		}
	}
	return blockerb;
}

inline U64 random_64int() {
	
	std::random_device rd;
	std::uniform_int_distribution<U64> distribution(0, 18446744073709551615);
	std::mt19937 mt(rd());

	return distribution(mt);
}

inline U64 sparse_random() {
	return random_64int() & random_64int() & random_64int();
}

U64 find_magic_Rook(Square sq) {
	
	U64 used[1024] = {0};

	int index;

	U64 mask = Rook_all(sq);
	int numbits = popcount(mask);
	bool badmagic = false;
	
	for (int g = 0; g < 100000; g++) {
		
		badmagic = true;
		U64 magic = sparse_random();
		//U64 magic = 0x48FFFE99FECFAA00;

		for (int k = 0; k < 1024; k++) {
			used[k] = 0;
		}
		
		

		for (int i = 0, fail = 0; !fail && i < (1 << numbits); i++) {
			index = ((Rook_blockers[sq][i] * magic) >> (64 - numbits));
			if (used[index] == 0) { 
				used[index] = Rook_moves[sq][i];
				
			}
			else if (used[index] != Rook_moves[sq][i]) {
				badmagic = true;
				if (i > 900) {
					std::cout << "i: " << i << "   |   magic: " << magic <<"\n";
				}
				fail = 1;
			}
			
		}
		if (!badmagic) {
			return magic;
		}
	}
	return 0;
}

//function for generating all possible blocker masks for each square as well as the corresponding moveboard, for rooks
void magic_rook_init() {
	
	bool magicfound = false;
	std::ofstream Rmagics("Rmagics.txt");

	for (int sq = 0; sq < 64; sq++) {

		U64 blocker_mask = Rook_all(Square(sq));
		int num_bits_rook_mask = popcount(blocker_mask);
		for (int i = 0; i < (1 << num_bits_rook_mask); i++) {
			U64 uniq_blockb = generate_blocker(i, blocker_mask);
			Rook_blockers[sq][i] = uniq_blockb;
			Rook_moves[sq][i] = Rook_movebb(Square(sq), uniq_blockb);
		}
		show_bitboard(blocker_mask);
		std::cout << "square: " << sq << "    numbits: " << num_bits_rook_mask << "    2^bits: " << (1 << num_bits_rook_mask) << "\n";

	}

}
//function for generating all possible blocker masks for each square as well as the corresponding moveboard, for bishops
void magic_bish_init() {

	for (int sq = 0; sq < 64; sq++) {

		U64 blocker_mask = Bish_all(Square(sq));
		int num_bits_bish_mask = popcount(blocker_mask);
		for (int i = 0; i < (1 << num_bits_bish_mask); i++) {
			U64 blockerboard = generate_blocker(i, blocker_mask);
			Bish_blockers[sq][i] = blockerboard;
			Bish_moves[sq][i] = Bish_movebb(Square(sq), blockerboard);
		}
	}
}