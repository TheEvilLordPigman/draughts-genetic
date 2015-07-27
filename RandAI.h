/*
 * RandAI.h
 *
 *  Created on: 27 Jul 2015
 *      Author: Jack
 */

#ifndef RANDAI_H_
#define RANDAI_H_

#include "DraughtsAI.h"

class RandAI : public AI {
public:
	RandAI(cellState colour);
	MoveSequence getMove(Board, int depth, bool nodeType = N_MAX);
};

#endif /* RANDAI_H_ */
