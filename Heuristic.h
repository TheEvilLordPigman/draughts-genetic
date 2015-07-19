/*
 * Heuristic.h
 *
 *  Created on: 17 Jun 2015
 *      Author: Jack
 */

#ifndef HEURISTIC_H_
#define HEURISTIC_H_

#include "DraughtsModule.h"
#include "BinConvert.h"
#include <sstream>

enum HeuristicParameter {
	PARAM_MEN,
	PARAM_KINGS,
	PARAM_ENEMY_MEN,
	PARAM_ENEMY_KINGS,
	PARAM_SAFE_MEN,
	PARAM_SAFE_KINGS,
	PARAM_MEN_DIST,
	PARAM_PROMOTION_SPACE,
	PARAM_WIN,
	PARAM_TOTAL
};

class Heuristic {
private:
	float values[PARAM_TOTAL];

public:
	Heuristic();
	double function(Board board);
	void setValue(HeuristicParameter param, float val);
	std::string toString();
	std::vector<bool> getChromosome();
};



#endif /* HEURISTIC_H_ */
