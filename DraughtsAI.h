/*
* Header file for AI functions
*/

#ifndef AI_H_
#define AI_H_

#include "DraughtsModule.h"
#include "Heuristic.h"
#include <stdlib.h>
#include <time.h>

const int SEARCH_DEPTH = 4;
const bool N_MAX = true;
const bool N_MIN = false;

//Returns the heuristic desirability value for the board state
//double heuristic(Board);

//Heuristic parameter coefficients (arbitrary)
const double PARAM_MULT[PARAM_TOTAL] = {
	1.0,
	2.0,
	-1.0,
	-2.0,
	0.25,
	0.5,
	-0.001,
	0.5,
	10
};

class AI {
	private:
		cellState colour;
		int direction;
		int searchDepth;
		Heuristic heuristic;

	public:
		AI(cellState playercolour, Heuristic heur);
		cellState getColour();
		void setSearchDepth(int depth);
		int getSearchDepth();

		//Returns a vector containing all legal moves on board
		std::vector<MoveSequence> getAvailableMoves(Board);

		void gatherCaptures(Board board, DraughtsMove currentmove, std::vector<MoveSequence>&);

		//Returns a list of available captures to the piece at Cell on Board
		std::vector<MoveSequence> getAvailableCapturesFromPoint(Board,Cell);

		//Returns what the AI thinks is the optimal move given the board state
		MoveSequence getMove(Board, int depth, bool nodeType = N_MAX);
};

#endif

