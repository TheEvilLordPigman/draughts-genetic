/*
* Header file for the board.cells, cell and move classes and functions to validate moves and
* other useful things used by both the main game and the AI.
*/

#ifndef DRAUGHTSMODULE_H
#define DRAUGHTSMODULE_H

#include <vector> //For vector
#include <fstream> //For ifstream
#include <string>

#define BOARD_WIDTH 8
#define BOARD_HEIGHT 8

//Each possible state for a board.cells cell
enum cellState {EMPTY,M_WHITE,K_WHITE,M_BLACK,K_BLACK};

//Converts a king to a man if a king is passed to the function
cellState DeKing(cellState cell);

//Returns the opposite player to the one given
cellState otherplayer(cellState);

//Represents the board
struct Board {
	cellState cells[8][8];
};

//Represents a board cell
struct Cell {
	int x;
	int y;
	Cell() {}
	Cell(int X,int Y): x(X),y(Y) {}
};

//Represents a move
class DraughtsMove {
	public:
		Cell start;
		Cell destination;

		DraughtsMove();
		DraughtsMove(Cell strt, Cell dest, bool cap = false);

		//~DraughtsMove();

		//DraughtsMove* nextMove;

		bool capture; //True if the move is a capture
		bool isValid(Board,cellState); //Returns true if the move is a valid draughts move
		std::vector<Cell> getAffectedCells(); //Returns a vector containing all of the cells the move affects
};

//A struct for a series of moves in succession (serial captures)
struct MoveSequence {
	std::vector<DraughtsMove> moves;
	MoveSequence(DraughtsMove first);
	MoveSequence(): exists(false) {};
	bool exists;
};

//Returns true if the piece at (x,y) on the board can capture
bool CanCapture(Board,int x,int y);

//Returns true if a capture is available to the player
bool PlayerCanCapture(Board,cellState player);

//Returns true if the cell is within the board boundaries
bool CellInBounds(Cell);

//Returns true if player has won the game
bool HasWon(cellState player, Board board);

Board ExecuteMove(DraughtsMove move, Board board);
Board ExecuteMoveSequence(MoveSequence moves, Board board);

void ResetBoard(Board &board);

#endif
