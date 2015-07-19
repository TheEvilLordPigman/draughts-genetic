/*
* Implementation of the common functions and classes of the game and AI
*/

#include "DraughtsModule.h"

#define BOARD_WIDTH 8
#define BOARD_HEIGHT 8

cellState DeKing(cellState cell) {
	if((cell != M_WHITE)&&(cell != M_BLACK)&&(cell != EMPTY)) {
		return cellState((int)cell-1);
	}
	else {
		return cell;
	}
}

cellState otherplayer(cellState playercolour) {
	return cellState(4-((int)DeKing(playercolour)));
}

DraughtsMove::DraughtsMove() {
	capture = false;
}

DraughtsMove::DraughtsMove(Cell strt, Cell dest, bool cap) {
	start = strt;
	destination = dest;
	capture = cap;
}

//DraughtsMove::~DraughtsMove() {
//	delete nextMove;
//	nextMove = nullptr;
//}

//I hope you like horribly long boolean expressions...
bool DraughtsMove::isValid(Board board, cellState playercolour){

	bool playercancapture = PlayerCanCapture(board,playercolour);

	int direction;
	if(playercolour == M_BLACK) {
		direction = 1;
	}
	else {
		direction = -1;
	}

	//Man normal move
	if((start.y == destination.y + direction) && ((start.x == destination.x - 1)||(start.x == destination.x + 1)) && !playercancapture) {
		return true;
	}
	//King normal move backwards
	else if(((start.y == destination.y - direction) && ((start.x == destination.x - 1)||(start.x == destination.x + 1))) && (board.cells[start.y][start.x] == playercolour+1) && (!playercancapture)){
		return true;
	}
	//Man captures
	else if((start.y == destination.y + direction*2) && ((start.x == destination.x - 2)||(start.x == destination.x + 2))) {
		if((board.cells[(start.y + destination.y)/2][(start.x + destination.x)/2] == otherplayer(playercolour))||(board.cells[(start.y + destination.y)/2][(start.x + destination.x)/2] == cellState((int)otherplayer(playercolour)+1))){
			capture = true;
			return true;
		}
		else {
			return false;
		}
		
	}
	//King captures backwards
	else if(((start.y == destination.y - direction*2) && ((start.x == destination.x - 2)||(start.x == destination.x + 2)))&&(board.cells[start.y][start.x] == playercolour+1)) {
		if((board.cells[(start.y + destination.y)/2][(start.x + destination.x)/2] == otherplayer(playercolour))||(board.cells[(start.y + destination.y)/2][(start.x + destination.x)/2] == cellState((int)otherplayer(playercolour)+1))){
			capture = true;
			return true;
		}
		else {
			return false;
		}
		
	}
	else {
		return false;
	}
};

std::vector<Cell> DraughtsMove::getAffectedCells() {
	std::vector<Cell> cells;
	cells.reserve(3);
	cells.push_back(start);
	cells.push_back(destination);
	
	if(capture) {
		Cell capturedcell;
		capturedcell.x = (start.x + destination.x)/2;
		capturedcell.y = (start.y + destination.y)/2;
		cells.push_back(capturedcell);
	}
	return cells;
}

MoveSequence::MoveSequence(DraughtsMove first) {
	moves.push_back(first);
	exists = true;
}

bool CanCapture(Board board, int x, int y) {
	//Checks to see if the cell is a king straight away, to make later code neater...
	bool isKing = false;
	if((board.cells[y][x] == K_WHITE) || (board.cells[y][x] == K_BLACK)) {
		isKing = true;
	}

	int direction = 0;
	if(board.cells[y][x] == M_BLACK || board.cells[y][x] == K_BLACK) {
		direction = -1;
	}
	else if(board.cells[y][x] == M_WHITE || board.cells[y][x] == K_WHITE) {
		direction = 1;
	}

	//...but it's still pretty horrid. Meh.
	if((DeKing(board.cells[y+direction][x+1]) == otherplayer(DeKing(board.cells[y][x])))&&((board.cells[y+direction*2][x+2] == EMPTY)&&(CellInBounds(Cell(x+2,y+direction*2))))) {
		return true;
	}
	else if((DeKing(board.cells[y+direction][x-1]) == otherplayer(DeKing(board.cells[y][x])))&&((board.cells[y+direction*2][x-2] == EMPTY)&&(CellInBounds(Cell(x-2,y+direction*2))))) {
		return true;
	}
	else if((DeKing(board.cells[y-direction][x+1]) == otherplayer(DeKing(board.cells[y][x])))&&((board.cells[y-direction*2][x+2] == EMPTY)&&(CellInBounds(Cell(x+2,y-direction*2))))&&(isKing)) {
		return true;
	}
	else if((DeKing(board.cells[y-direction][x-1]) == otherplayer(DeKing(board.cells[y][x])))&&((board.cells[y-direction*2][x-2] == EMPTY)&&(CellInBounds(Cell(x-2,y-direction*2))))&&(isKing)) {
		return true;
	}
	else {
		return false;
	}

}

bool PlayerCanCapture(Board board, cellState player) {
	bool captureavailable = false;
	for(int y=0; y<BOARD_HEIGHT; y++) {
		for(int x=0; x<BOARD_WIDTH; x++) {
			//If the current cell is one of the player's men or kings and it can capture, return true.
			if((board.cells[y][x] == player)||(board.cells[y][x] == player+1)) {
				if(CanCapture(board,x,y)) {
					captureavailable = true;
				}
			}
		}
	}
	return captureavailable;
}

bool CellInBounds(Cell cell) {
	if(cell.x < BOARD_WIDTH && cell.x >= 0 && cell.y < BOARD_HEIGHT && cell.y >= 0) {
		return true;
	}
	else {
		return false;
	}
}

bool HasWon(cellState player, Board board) {
	//Get the opponent's colour
	cellState opponent = otherplayer(player);
	bool won = true;

	//Get the direction the opponent can move in
	int direction;
	if(opponent == M_BLACK) {
		direction = -1;
	} else {
		direction = 1;
	}

	/*
	In draughts, a player wins if their opponent cannot move. This code therefore
	checks to see if there are any possible moves for the player's opponent.
	*/

	for(int y=0; y<BOARD_HEIGHT; y++) {
		for(int x=0; x<BOARD_WIDTH; x++) {
			//If the current space is the opponent's man or king
			if(board.cells[y][x] == opponent || board.cells[y][x] == opponent + 1) {
				//Can it move normally?
				if(board.cells[y+direction][x+1] == EMPTY || board.cells[y+direction][x-1] == EMPTY) {
					won = false;
				}

				//If it is a king, can it move backwards?
				if(board.cells[y][x] == opponent + 1 && (board.cells[y-direction][x+1] == EMPTY || board.cells[y-direction][x-1] == EMPTY)) {
					won = false;
				}

				//Can it capture?
				if(CanCapture(board,x,y)) {
					won = false;
				}
				
			}
		}
	}
	return won;
}

Board ExecuteMove(DraughtsMove move, Board board) {
	Board modBoard = board;
	modBoard.cells[move.destination.y][move.destination.x] = modBoard.cells[move.start.y][move.start.x];
	modBoard.cells[move.start.y][move.start.x] = EMPTY;

	if(move.capture) {
		//Delete the piece that has been captured
		modBoard.cells[(move.start.y + move.destination.y)/2][(move.start.x + move.destination.x)/2] = EMPTY;
	}

	if(modBoard.cells[move.destination.y][move.destination.x] == M_BLACK && move.destination.y == 0) {
		modBoard.cells[move.destination.y][move.destination.x] = K_BLACK;
	}
	if(modBoard.cells[move.destination.y][move.destination.x] == M_WHITE && move.destination.y == BOARD_HEIGHT-1) {
		modBoard.cells[move.destination.y][move.destination.x] = K_WHITE;
	}

	return modBoard;
}

Board ExecuteMoveSequence(MoveSequence sequence, Board board) {
	Board finalboard = board;
	for(unsigned int i=0; i<sequence.moves.size(); i++) {
		finalboard = ExecuteMove(sequence.moves[i],finalboard);
	}
	return finalboard;
}

void ResetBoard(Board& board) {
	//Opens the initial position file
	std::ifstream startposfile("startpos.txt");
	std::string currentline;
	int currentval;

	//Set the board's values to those of the file
	for(int y=0; y<BOARD_HEIGHT; y++) {
		if(std::getline(startposfile,currentline)) {
			for(int x=0; x<BOARD_WIDTH; x++) {
				currentval = currentline[x]-48;
				board.cells[y][x] = cellState(currentval);
			}
		}
	}
	startposfile.close();
}
