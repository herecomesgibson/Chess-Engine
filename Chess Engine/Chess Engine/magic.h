#pragma once


#include <iostream>
#include <fstream>
#include <random>
#include <iomanip>
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
	72057594037927936,
};
const U64 NW_SE_WHITEdiagonal[7] = {
	0x102,
	0x1020408,
	0x10204081020,
	0x102040810204080,
	0x408102040800000,
	0x1020408000000000,
	4647714815446351872,
};


U64 Rook_blockers[64][4096];
U64 Rook_moves[64][4096];
U64 Rook_magic_moves[64][4096];

U64 Bish_blockers[64][1024];
U64 Bish_moves[64][1024];
U64 Bish_magic_moves[64][1024]; // This is to put movebaords into their new location for magic indexing, I think its redundant since we could just use Bish_moves but I want to have the backup untill I get Bish moves working

const U64 Rook_magics[64] = {
	0x2480002040001688,
	0x400040200b1004,
	0x80088610002000,
	0x800c8008011000,
	0x2200100420020018,
	0x500130004000218,
	0x100008900040200,
	0x4900002042008100,

	0x2400800080400821,
	0x200401000402008,
	0x80805000842000,
	0x802000920160040,
	0xe001004200a00,
	0x810a0010080e0004,
	0x8002001200280401,
	0x24800100004180,

	0x20208000804008,
	0x4044210040038102,
	0x2001010040102001,
	0x280420020901a00,
	0x4001010004100800,
	0x1808024000200,
	0x40005023008,
	0x41802a000040a401,

	0xc0688080004000,
	0x50004c40002008,
	0x48200500401100,
	0x8000300180280280,
	0x210c880080800400,
	0x3040080800200,
	0xc010021400483001,
	0x140244020000a041,

	0x2224000800882,
	0x946050082002440,
	0x839004802001,
	0x242200400a001024,
	0x480800800804400,
	0x402000846000411,
	0x10224001008,
	0x41010066000084,

	0x3040006080448000,
	0x10002010484004,
	0x10208292020040,
	0x102005040220008,
	0x112005004620008,
	0x2442040480110,
	0x4120088110040006,
	0x208114184020021,

	0x210800100403100,
	0x1000420420810200,
	0x41809000600080,
	0x10420069a200,
	0x140c040280280080,
	0x8a001450484200,
	0x48810810020400,
	0x40010880441600,

	0x40008004d0c02901,
	0x80c5044210a48202,
	0x5c8c220015101,
	0x8810010038042031,
	0x10004100a0801,
	0x1005008400080229,
	0x8108510081204,
	0x3040024010a4182,
};


U64 Bish_magics[64] = {
	0x40580905082102,
	0x1008010820850000,
	0x4290401000001,
	0x308248502008820,
	0x882a061001202081,
	0x80a0804041010a8,
	0x4010450242004,
	0x6202108184040,

	0x92004210040108c0,
	0x800040404140020,
	0x8020080200421040,
	0x20100a2082048041,
	0x24051040000000,
	0x8040020804050000,
	0x1008811098244021,
	0x480488404028202,

	0x2008221090,
	0x4010042802018400,
	0x1001c204144278,
	0x4058000082024080,
	0x92008c010c1000,
	0x1141004090080120,
	0x140502402021050,
	0x4000811644480802,

	0x200082e010905044,
	0x20b010420200,
	0x8040018005010,
	0x1203040008020820,
	0x111010040104002,
	0x109001202411d,
	0xd002020901480254,
	0x4029021004120,

	0x9a00009801d0208,
	0x2080004808100281,
	0x200020200010800,
	0x11180a0084080080,
	0x80c0004010010101,
	0x8411004100020101,
	0x2002060050440410,
	0x32c02418020440c,

	0x2020110008000114,
	0x1000101100c2142,
	0x8020000132040808,
	0x8286002234004800,
	0x2201021040442400,
	0x201020ca1000200,
	0x20022602022046,
	0x401020200000a,

	0x8001404000,
	0x80000000031d2000,
	0x80110080206108a,
	0xa0a020848,
	0x800004011410200,
	0x181010188000,
	0x4020021001010000,
	0x1020082a34444218,

	0x9128084011004,
	0x1340020241,
	0x201040000024200,
	0x4002080000a08848,
	0x10b0406404050408,
	0x404a44204888202,
	0x4000041808880880,
	0x8002201204010021,
};



//blocker mask of all potential rook moves - edge moves. If exclude_edges is set to false, the edge bits will be included in the output
//NOTE: these two functions could be replaced by lookup tables at some point to improve efficiency
U64 Rook_all(Square sq, bool exclude_edges = true) {
	U64 bedges = board_edges;
	U64 moves = rankofbb(sq) | fileofbb(sq);
	if (exclude_edges == true) {
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

//returns a blocker mask for a bishop on any given square(possible unblocked moves)
U64 Bish_all(Square sq, bool edges = true) {

	U64 retboard = 0;

	U64 bit = single_bitboards[sq];

	for (int i = 0; i < 8; i++) {
		if (NW_SE_BLACKdiagonal[i] & bit) { retboard |= NW_SE_BLACKdiagonal[i]; }
		if (NE_SW_WHITEdiagonal[i] & bit) { retboard |= NE_SW_WHITEdiagonal[i]; }
	}
	for (int i = 0; i < 7; i++) {
		if (NE_SW_BLACKdiagonal[i] & bit) { retboard |= NE_SW_BLACKdiagonal[i]; }
		if (NW_SE_WHITEdiagonal[i] & bit) { retboard |= NW_SE_WHITEdiagonal[i]; }
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

U64 find_magic_Bish(Square sq) {
	
	U64 used[4096] = { 0 };

	int index;

	U64 mask = Bish_all(sq);
	int numbits = popcount(mask);
	bool badmagic = false;
	
	for (int g = 0; g < 1000000; g++) {
		
		badmagic = false;
		U64 magic = sparse_random();

		for (int k = 0; k < 4096; k++) {
			used[k] = 0;
		}
		
		for (int i = 0, fail = 0; !fail && i < (1 << numbits); i++) {
			index = ((Bish_blockers[sq][i] * magic) >> (64 - numbits));
			if (used[index] == 0) {

				used[index] = Bish_moves[sq][i];
				
			}
			else if (used[index] != Bish_moves[sq][i]) {
				badmagic = true;
				//if (i > 1000) {
					//std::cout << "i: " << i << "   |   magic: " << magic << "\n";
				//}
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
			U64 movebb = Rook_movebb(Square(sq), uniq_blockb);
			int magic_index = (uniq_blockb * Rook_magics[sq]) >> (64 - num_bits_rook_mask);

			Rook_blockers[sq][i] = uniq_blockb;
			Rook_moves[sq][i] = movebb;
			Rook_magic_moves[sq][magic_index] = movebb;
		}
	}

}
//function for generating all possible blocker masks for each square as well as the corresponding moveboard, for bishops
void magic_bish_init() {

	for (int sq = 0; sq < 64; sq++) {

		U64 blocker_mask = Bish_all(Square(sq));
		int num_bits_bish_mask = popcount(blocker_mask);
		for (int i = 0; i < (1 << num_bits_bish_mask); i++) {
			
			U64 blockerboard = generate_blocker(i, blocker_mask);
			U64 movebb = Bish_movebb(Square(sq), blockerboard);
			int magic_index = (blockerboard * Bish_magics[sq]) >> (64 - num_bits_bish_mask);

			Bish_blockers[sq][i] = blockerboard;
			Bish_moves[sq][i] = movebb;
			Bish_magic_moves[sq][magic_index] = movebb;

		}
	}
}
void print_magics_Bish() {
	U64 magic;
	for (int i = 0; i < 64; i++) {
		magic = find_magic_Bish(Square(i));
		std::cout << "0x" << std::hex << magic << "," << "\n";
	}
}