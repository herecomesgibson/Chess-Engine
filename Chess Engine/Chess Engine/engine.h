#include <iostream>
#include <bitset>
#include "Board.h"
#include "Types.h"



//class for consolidating a movelst and making it easier to iterate over for the perft function
class MoveIter {
public:
	explicit MoveIter(Chess::Board b) : last(b.generate_legal_moves(list)) {}
	const Move* begin() const { return list; }
	const Move* end() const { return last; }
	size_t size() const { return last - list; }

private:
	Move list[256];
	Move* last;
};