/*
 * Population.cpp
 *
 *  Created on: 25 Jul 2015
 *      Author: Jack
 */

#include "Population.h"

Population::Population() {
	generations = 0;

	parent1Index = 0;
	parent2Index = 0;

	pool.reserve(POP_SIZE);

	for(int i=0; i<POP_SIZE; i++) {
		pool.push_back(Heuristic());
		for(int j=0; j<PARAM_TOTAL; j++) {
			pool[i].setValue(HeuristicParameter(j), (rand() % 30) - 15);
		}
	}

	control.setValue(PARAM_MEN, 			1.0);
	control.setValue(PARAM_KINGS, 			2.0);
	control.setValue(PARAM_ENEMY_MEN, 		-1.0);
	control.setValue(PARAM_ENEMY_KINGS, 	-2.0);
	control.setValue(PARAM_SAFE_MEN, 		0.25);
	control.setValue(PARAM_SAFE_KINGS, 		0.5);
	control.setValue(PARAM_MEN_DIST, 		-0.001);
	control.setValue(PARAM_PROMOTION_SPACE, 0.5);
	control.setValue(PARAM_WIN, 			10);
}

void Population::assignFitness() {
	for(std::vector<Heuristic>::iterator it = pool.begin(); it != pool.end(); it++) {
		it->setFitness(findFitness(*it));
	}
}

void Population::selectParents() {
	std::vector<int> ballot;

	for(unsigned int i=0; i<pool.size(); i++) {
		for(int j=0; j<pool[i].getFitness(); j++) {
			ballot.push_back(i);
		}
	}
	parent1Index = ballot[rand() % ballot.size()];
	parent2Index = ballot[rand() % ballot.size()];
	while(parent2Index == parent1Index) {
		parent2Index = ballot[rand() % ballot.size()];
	}
}
void Population::breed(Heuristic parent1, Heuristic parent2, Heuristic* baby1, Heuristic* baby2) {
		std::vector<bool> parent1Chrom = parent1.getChromosome();
		std::vector<bool> parent2Chrom = parent2.getChromosome();
		int swapPoint = rand() % parent1Chrom.size();

		std::vector<bool> parent1ChromA = splice(parent1Chrom, 0, swapPoint);
		std::vector<bool> parent1ChromB = splice(parent1Chrom, swapPoint, parent1Chrom.size());

		std::vector<bool> parent2ChromA = splice(parent2Chrom, 0, swapPoint);
		std::vector<bool> parent2ChromB = splice(parent2Chrom, swapPoint, parent2Chrom.size());

		std::vector<bool> baby1Chrom = parent1ChromA;
		baby1Chrom.insert(baby1Chrom.end(), parent2ChromB.begin(), parent2ChromB.end());

		std::vector<bool> baby2Chrom = parent2ChromA;
		baby2Chrom.insert(baby2Chrom.end(), parent1ChromB.begin(), parent1ChromB.end());

		baby1->setFromChromosome(baby1Chrom);
		baby2->setFromChromosome(baby2Chrom);
}
void Population::breedNextGen() {
	std::vector<Heuristic> newPool;
	newPool.reserve(POP_SIZE);

	for(int i=0; i<(POP_SIZE/2); i++) {
		Heuristic a, b;
		breed(pool[parent1Index], pool[parent2Index], &a, &b);
		newPool.push_back(a);
		newPool.push_back(b);
	}

	pool = newPool;
	generations++;
}

void Population::printPop() {
	std::cout << "Generation " << generations << ":" << std::endl;
	for(std::vector<Heuristic>::iterator it = pool.begin(); it != pool.end(); it++) {
		std::cout << it->toString() << " - " << it->getFitness() << std::endl;
	}
}

void Population::printBoard(Board board) {
	std::cout << "-----------------" << std::endl;
	for(int y=0; y<BOARD_HEIGHT; y++) {
		std::cout << "|";
		for(int x=0; x<BOARD_WIDTH; x++) {
			if(board.cells[y][x] != 0) {
				std::cout << board.cells[y][x];
			}
			else {
				std::cout << " ";
			}
			std::cout << "|";
		}
		std::cout << std::endl << "-----------------" << std::endl;

	}
	std::cout << std::endl;
}

int Population::findFitness(Heuristic a) {
	AI black(M_BLACK, control);
	AI white(M_WHITE, a);
	bool won = false;
	bool turn = BLACK_PLAYER;

	Board board;
	ResetBoard(board);

	int turns = 0;

	while(!won && turns < MAX_TURNS) {
		if(turn) {
			board = ExecuteMoveSequence(black.getMove(board, SEARCH_DEPTH), board);
			//std::cout << "Black has made a move." << std::endl;
			//printBoard(board);
			if(HasWon(black.getColour(), board)) {
				won = true;
			}
		}
		else {
			board = ExecuteMoveSequence(white.getMove(board, SEARCH_DEPTH), board);
			//std::cout << "White has made a move." << std::endl;
			//printBoard(board);
			if(HasWon(white.getColour(), board)) {
				won = true;
			}
		}
		turn = !turn;
		turns++;
	}

	if(turns == MAX_TURNS) {
		//std::cout << "Turn limit reached, white wins because racism." << std::endl;
	}

	int score = 0;
	for(int y=0; y<8; y++) {
		for(int x=0; x<8; x++) {
			if(board.cells[y][x] == M_WHITE) {
				score += 1;
			}
			else if(board.cells[y][x] == K_WHITE) {
				score += 2;
			}
		}
	}

	return score;
}
