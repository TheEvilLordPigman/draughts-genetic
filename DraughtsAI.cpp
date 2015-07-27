/*
* Draughts AI implementation
*/

#include "DraughtsAI.h"
#include <iostream>

AI::AI() {

}

AI::AI(cellState playercolour, Heuristic heur) {
	colour = playercolour;
	if(colour == M_WHITE) {
		direction = 1;
	} else {
		direction = -1;
	}
	//srand(time(0));
	searchDepth = 0;
	heuristic = heur;
}

cellState AI::getColour() {
	return colour;
}

void AI::setSearchDepth(int depth) {
	searchDepth = depth;
}

int AI::getSearchDepth() {
	return searchDepth;
}

void AI::gatherCaptures(Board board, DraughtsMove currentmove, std::vector<MoveSequence>& movelist) {
	Board nextboard = ExecuteMove(currentmove,board);

	if(CanCapture(nextboard,currentmove.destination.x,currentmove.destination.y) && (board.cells[currentmove.start.y][currentmove.start.x] == nextboard.cells[currentmove.destination.y][currentmove.destination.x])) {
		std::vector<MoveSequence> nextcaptures = getAvailableCapturesFromPoint(nextboard,Cell(currentmove.destination.x,currentmove.destination.y));
		for(unsigned int i=0; i<nextcaptures.size(); i++) {
			MoveSequence currentseq = nextcaptures[i];
			currentseq.moves.insert(currentseq.moves.begin(),currentmove);
			movelist.push_back(currentseq);
			
		}
		
	}
	else {
		movelist.push_back(MoveSequence(currentmove));
	}
}

std::vector<MoveSequence> AI::getAvailableMoves(Board board) {
	std::vector<MoveSequence> movelist;
	bool cancapture = PlayerCanCapture(board,colour);

	for(int y=0; y<BOARD_HEIGHT; y++) {
		for(int x=0; x<BOARD_WIDTH; x++) {
			if(board.cells[y][x] == colour || board.cells[y][x] == colour+1) {
				if(cancapture) {
					//Normal captures
					if((DeKing(board.cells[y+direction][x+1]) == otherplayer(colour))&&((board.cells[y+direction*2][x+2] == EMPTY)&&(CellInBounds(Cell(x+2,y+direction*2))))) {
						gatherCaptures(board,DraughtsMove(Cell(x,y),Cell(x+2,y+direction*2),true),movelist);
					}
					else if((DeKing(board.cells[y+direction][x-1]) == otherplayer(colour))&&((board.cells[y+direction*2][x-2] == EMPTY)&&(CellInBounds(Cell(x-2,y+direction*2))))) {
						gatherCaptures(board,DraughtsMove(Cell(x,y),Cell(x-2,y+direction*2),true),movelist);
					}

					if(board.cells[y][x] == colour+1) {
						//King captures
						if((DeKing(board.cells[y-direction][x+1]) == otherplayer(colour))&&((board.cells[y-direction*2][x+2] == EMPTY)&&(CellInBounds(Cell(x+2,y-direction*2))))) {
							gatherCaptures(board,DraughtsMove(Cell(x,y),Cell(x+2,y-direction*2),true),movelist);
						}
						else if((DeKing(board.cells[y-direction][x-1]) == otherplayer(colour))&&((board.cells[y-direction*2][x-2] == EMPTY)&&(CellInBounds(Cell(x-2,y-direction*2))))) {
							gatherCaptures(board,DraughtsMove(Cell(x,y),Cell(x-2,y-direction*2),true),movelist);
						}
					}

				} else {
					//Normal moves
					if(board.cells[y+direction][x+1] == EMPTY && CellInBounds(Cell(x+1,y+direction))) {
						movelist.push_back(MoveSequence(DraughtsMove(Cell(x,y),Cell(x+1,y+direction))));
					}
					if(board.cells[y+direction][x-1] == EMPTY && CellInBounds(Cell(x-1,y+direction))) {
						movelist.push_back(MoveSequence(DraughtsMove(Cell(x,y),Cell(x-1,y+direction))));
					}

					if(board.cells[y][x] == colour+1) {
						//King normal moves
						if(board.cells[y-direction][x+1] == EMPTY && CellInBounds(Cell(x+1,y-direction))) {
							movelist.push_back(MoveSequence(DraughtsMove(Cell(x,y),Cell(x+1,y-direction))));
						}
						if(board.cells[y-direction][x-1] == EMPTY && CellInBounds(Cell(x-1,y-direction))) {
							movelist.push_back(MoveSequence(DraughtsMove(Cell(x,y),Cell(x-1,y-direction))));
						}
					}
				}
			}
		}
	}
	return movelist;
}

std::vector<MoveSequence> AI::getAvailableCapturesFromPoint(Board board, Cell cell) {
	//Normal captures
	int x = cell.x;
	int y = cell.y;
	std::vector<MoveSequence> movelist;

	if((DeKing(board.cells[y+direction][x+1]) == otherplayer(colour))&&((board.cells[y+direction*2][x+2] == EMPTY)&&(CellInBounds(Cell(x+2,y+direction*2))))) {
		gatherCaptures(board,DraughtsMove(Cell(x,y),Cell(x+2,y+direction*2),true),movelist);
	}
	else if((DeKing(board.cells[y+direction][x-1]) == otherplayer(colour))&&((board.cells[y+direction*2][x-2] == EMPTY)&&(CellInBounds(Cell(x-2,y+direction*2))))) {
		gatherCaptures(board,DraughtsMove(Cell(x,y),Cell(x-2,y+direction*2),true),movelist);
	}

	if(board.cells[y][x] == colour+1) {
		//King captures
		if((DeKing(board.cells[y-direction][x+1]) == otherplayer(colour))&&((board.cells[y-direction*2][x+2] == EMPTY)&&(CellInBounds(Cell(x+2,y-direction*2))))) {
			gatherCaptures(board,DraughtsMove(Cell(x,y),Cell(x+2,y-direction*2),true),movelist);
		}
		else if((DeKing(board.cells[y-direction][x-1]) == otherplayer(colour))&&((board.cells[y-direction*2][x-2] == EMPTY)&&(CellInBounds(Cell(x-2,y+direction*2))))) {
			gatherCaptures(board,DraughtsMove(Cell(x,y),Cell(x-2,y-direction*2),true),movelist);
		}
	}
	return movelist;
}

//double heuristic(Board board) {
//	cellState colour = M_WHITE;
//
//	//Initialise parameter array
//	int parameters[PARAM_TOTAL];
//	for(int i=0; i<PARAM_TOTAL; i++) {
//		parameters[HeuristicParameter(i)] = 0;
//	}
//
//	//Get parameters
//	for(int y=0; y<BOARD_HEIGHT; y++) {
//		for(int x=0; x<BOARD_WIDTH; x++) {
//			if(board.cells[y][x] == colour) {
//				parameters[PARAM_MEN] += 1;
//				if(x == 0 || x == BOARD_WIDTH-1) {
//					parameters[PARAM_SAFE_MEN] += 1;
//				}
//				if(colour == M_WHITE) {
//					parameters[PARAM_MEN_DIST] += BOARD_HEIGHT-y;
//				}
//				else {
//					parameters[PARAM_MEN_DIST] += y;
//				}
//			}
//			else if(board.cells[y][x] == colour+1) {
//				parameters[PARAM_KINGS] += 1;
//				if(x == 0 || x == BOARD_WIDTH-1) {
//					parameters[PARAM_SAFE_KINGS] += 1;
//				}
//			}
//			else if(board.cells[y][x] == otherplayer(colour)) {
//				parameters[PARAM_ENEMY_MEN] += 1;
//			}
//			else if(board.cells[y][x] == otherplayer(colour) + 1) {
//				parameters[PARAM_ENEMY_KINGS] += 1;
//			}
//			else if(x % 2 == 0) {
//				if(colour == M_WHITE && y == BOARD_HEIGHT-1) {
//					parameters[PARAM_PROMOTION_SPACE] += 1;
//				}
//				if(colour == M_BLACK && y == 0) {
//					parameters[PARAM_PROMOTION_SPACE] += 1;
//				}
//			}
//		}
//	}
//
//	if(HasWon(colour, board)) {
//		parameters[PARAM_WIN] = 1;
//	}
//
//	double result = 0.0;
//	for(int i=0; i<PARAM_TOTAL; i++) {
//		result += parameters[HeuristicParameter(i)] * PARAM_MULT[HeuristicParameter(i)];
//	}
//	return result;
//}

MoveSequence AI::getMove(Board board, int depth, bool nodeType) {
	std::vector<MoveSequence> movelist;
	movelist = getAvailableMoves(board);
	std::vector<double> desirabilities;
	desirabilities.reserve(movelist.size());
	for(unsigned int i=0; i<movelist.size(); i++) {
		if(depth == 0) {
			desirabilities.push_back(heuristic.function(ExecuteMoveSequence(movelist[i],board), colour));
		}
		else {
			AI playersim(otherplayer(colour), heuristic);
			desirabilities.push_back(heuristic.function(ExecuteMoveSequence(playersim.getMove(ExecuteMoveSequence(movelist[i],board),depth-1,!nodeType),board), colour));
		}
	}

	int bestindex = 0;
	
	double maxmin = 0.0;
	if(nodeType == N_MAX) {
		for(unsigned int i=0; i<desirabilities.size(); i++) {
			if(desirabilities[i] > maxmin) {
				maxmin = desirabilities[i];
				bestindex = i;
			}
		}
	}
	if(nodeType == N_MIN) {
		for(unsigned int i=0; i<desirabilities.size(); i++) {
			if(desirabilities[i] < maxmin) {
				maxmin = desirabilities[i];
				bestindex = i;
			}
		}	
	}

	if(movelist.size() > 0) {
		return movelist[bestindex];
	} else {
		return MoveSequence();
	}
}
