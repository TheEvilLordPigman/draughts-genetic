/*
 * BinConvert.h
 *
 *  Created on: 8 Jul 2015
 *      Author: Jack
 */

#ifndef BINCONVERT_H_
#define BINCONVERT_H_

#include <vector>
#include <cmath>
#include <iostream>

class BinConverter {
private:
	int bits;
	int dpIndex;
public:
	BinConverter(int bits, int dpIndex = 0);
	std::vector<bool> decToBin(int dec);
	std::vector<bool> decToBin(float dec);
	int binToDecInt(std::vector<bool> bin);
	float binToDecFloat(std::vector<bool> bin);
};

void printBinString(std::vector<bool> bin);

#endif /* BINCONVERT_H_ */
