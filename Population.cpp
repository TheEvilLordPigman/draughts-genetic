/*
 * Population.cpp
 *
 *  Created on: 25 Jul 2015
 *      Author: Jack
 */

#include "Population.h"

Population::Population() {
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
		it->setFitness(getFitness(*it));
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

void Population::printPop() {
	for(std::vector<Heuristic>::iterator it = pool.begin(); it != pool.end(); it++) {
		std::cout << it->toString() << std::endl;
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

int Population::getFitness(Heuristic a) {
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
			std::cout << "Black has made a move." << std::endl;
			printBoard(board);
			if(HasWon(black.getColour(), board)) {
				won = true;
			}
		}
		else {
			board = ExecuteMoveSequence(white.getMove(board, SEARCH_DEPTH), board);
			std::cout << "White has made a move." << std::endl;
			printBoard(board);
			if(HasWon(white.getColour(), board)) {
				won = true;
			}
		}
		turn = !turn;
		turns++;
	}

	if(turns == MAX_TURNS) {
		std::cout << "Turn limit reached, white wins because racism." << std::endl;
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
