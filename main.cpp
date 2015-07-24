/*
 * main.cpp
 *
 *  Created on: 12 Jun 2015
 *      Author: Jack
 */

#include "DraughtsAI.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

const bool BLACK_PLAYER = true;
const bool WHITE_PLAYER = false;

const int MAX_TURNS = 100;

void printBoard(Board board) {
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

bool getWinner(Heuristic b, Heuristic w) {
	AI black(M_BLACK, b);
	AI white(M_WHITE, w);
	bool won = false;
	bool turn = BLACK_PLAYER;
	bool winner = true;

	Board board;
	ResetBoard(board);

	int turns = 0;

	while(!won && turns < MAX_TURNS) {
		if(turn) {
			board = ExecuteMoveSequence(black.getMove(board, SEARCH_DEPTH), board);
			std::cout << "Black has made a move." << std::endl;
			printBoard(board);
			if(HasWon(black.getColour(), board)) {
				winner = BLACK_PLAYER;
				won = true;
			}
		}
		else {
			board = ExecuteMoveSequence(white.getMove(board, SEARCH_DEPTH), board);
			std::cout << "White has made a move." << std::endl;
			printBoard(board);
			if(HasWon(white.getColour(), board)) {
				winner = WHITE_PLAYER;
				won = true;
			}
		}
		turn = !turn;
		turns++;
	}

	if(turns == MAX_TURNS) {
		std::cout << "Turn limit reached, white wins because racism." << std::endl;
		winner = WHITE_PLAYER;
	}

	return winner;
}

int main(int argc, char* args[]) {
	srand(time(NULL));

	Heuristic A;
	for(int i=0; i<PARAM_TOTAL; i++) {
		A.setValue(HeuristicParameter(i), (rand() % 10) - 5);
	}
	std::vector<bool> AChrom = A.getChromosome();
	//printBinString(AChrom);

	Heuristic B;
	for(int i=0; i<PARAM_TOTAL; i++) {
		B.setValue(HeuristicParameter(i), (rand() % 10) - 5);
	}
	std::cout << B.toString() << std::endl;

	if(getWinner(A, B) == BLACK_PLAYER) {
		std::cout << "Black won." << std::endl;
	}
	else {
		std::cout << "White won." << std::endl;
	}
}


