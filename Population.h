/*
 * Population.h
 *
 *  Created on: 25 Jul 2015
 *      Author: Jack
 */

#ifndef POPULATION_H_
#define POPULATION_H_

#include "Heuristic.h"
#include "DraughtsAI.h"

const bool BLACK_PLAYER = true;
const bool WHITE_PLAYER = false;

const int MAX_TURNS = 100;

class Population {
private:
	std::vector<Heuristic> pool;
	Heuristic control;
	int parent1Index, parent2Index;

public:
	static const int POP_SIZE = 4;

	Population(); //Create random population
	Population(std::string fileName); //Load population from file

	void assignFitness();
	void selectParents();
	void breed();

	void printPop();
	void printBoard(Board board);
	int getFitness(Heuristic a);

};



#endif /* POPULATION_H_ */
