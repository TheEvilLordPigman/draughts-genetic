/*
 * RandAI.cpp
 *
 *  Created on: 27 Jul 2015
 *      Author: Jack
 */

#include "RandAI.h"

RandAI::RandAI(cellState colour) {
	this->colour = colour;
	if(colour == M_WHITE) {
		direction = 1;
	} else {
		direction = -1;
	}
}

MoveSequence RandAI::getMove(Board board, int depth, bool nodeType) {
	std::vector<MoveSequence> movelist = getAvailableMoves(board);
	return movelist[rand() % movelist.size()];
}
