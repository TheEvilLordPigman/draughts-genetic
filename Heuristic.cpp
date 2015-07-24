/*
 * Heuristic.cpp
 *
 *  Created on: 17 Jun 2015
 *      Author: Jack
 */

#include "Heuristic.h"

Heuristic::Heuristic() {
	for(int i=0; i<PARAM_TOTAL; i++) {
		values[i] = 0;
	}
}

double Heuristic::function(Board board) {
	cellState colour = M_WHITE;

	//Initialise parameter array
	int parameters[PARAM_TOTAL];
	for(int i=0; i<PARAM_TOTAL; i++) {
		parameters[HeuristicParameter(i)] = 0;
	}

	//Get parameters
	for(int y=0; y<BOARD_HEIGHT; y++) {
		for(int x=0; x<BOARD_WIDTH; x++) {
			if(board.cells[y][x] == colour) {
				parameters[PARAM_MEN] += 1;
				if(x == 0 || x == BOARD_WIDTH-1) {
					parameters[PARAM_SAFE_MEN] += 1;
				}
				if(colour == M_WHITE) {
					parameters[PARAM_MEN_DIST] += BOARD_HEIGHT-y;
				}
				else {
					parameters[PARAM_MEN_DIST] += y;
				}
			}
			else if(board.cells[y][x] == colour+1) {
				parameters[PARAM_KINGS] += 1;
				if(x == 0 || x == BOARD_WIDTH-1) {
					parameters[PARAM_SAFE_KINGS] += 1;
				}
			}
			else if(board.cells[y][x] == otherplayer(colour)) {
				parameters[PARAM_ENEMY_MEN] += 1;
			}
			else if(board.cells[y][x] == otherplayer(colour) + 1) {
				parameters[PARAM_ENEMY_KINGS] += 1;
			}
			else if(x % 2 == 0) {
				if(colour == M_WHITE && y == BOARD_HEIGHT-1) {
					parameters[PARAM_PROMOTION_SPACE] += 1;
				}
				if(colour == M_BLACK && y == 0) {
					parameters[PARAM_PROMOTION_SPACE] += 1;
				}
			}
		}
	}

	if(HasWon(colour, board)) {
		parameters[PARAM_WIN] = 1;
	}

	double result = 0.0;
	for(int i=0; i<PARAM_TOTAL; i++) {
		result += parameters[HeuristicParameter(i)] * values[HeuristicParameter(i)];
	}
	return result;
}

void Heuristic::setValue(HeuristicParameter param, float val) {
	values[param] = val;
}

std::string Heuristic::toString() {
	std::string result = "";
	std::stringstream ss;
	for(int i=0; i<PARAM_TOTAL; i++) {
		ss << values[i];
		result += ss.str() + ",";
		ss.str(std::string());
	}
	return result;
}

std::vector<bool> Heuristic::getChromosome() {
	std::vector<bool> result;
	BinConverter convert(15,8);
	for(int i=0; i<PARAM_TOTAL; i++) {
		std::vector<bool> current = convert.decToBin(values[i]);
		result.insert(result.end(), current.begin(), current.end());
	}
	return result;
}
