/*
 * BinConvert.cpp
 *
 *  Created on: 8 Jul 2015
 *      Author: Jack
 */

#include "BinConvert.h"

BinConverter::BinConverter(int bits, int dpIndex) {
	this->bits = bits+1;
	this->dpIndex = dpIndex;
}

std::vector<bool> BinConverter::decToBin(int dec) {
	std::cout << bits;
	std::vector<bool> result;
	result.reserve(bits);

	int currentDec = dec;
	if(dec < 0) {
		//Add a sign bit
		result.push_back(1);
		currentDec *= -1;
	} else {
		result.push_back(0);
	}

	while(currentDec != 0) {
		result.push_back(currentDec % 2);
		std::cout << currentDec << std::endl;
		currentDec /= 2;
	}
	if(result.size() < bits) {
		for(unsigned int i=0; i<bits-result.size(); i++) {
			result.push_back(false);
		}
	}
	return result;
}

std::vector<bool> BinConverter::decToBin(float dec) {
	int decInt = (int)floor(dec);
	float decFrac = dec - decInt;
	bits -= dpIndex;
	std::vector<bool> intPart = decToBin(decInt);
	bits += dpIndex;
	std::vector<bool> fracPart;
	for(int i=0; i<dpIndex; i++) {
		decFrac = decFrac*2;
		fracPart.push_back(floor(decFrac));
		decFrac -= floor(decFrac);
	}
	for(int i=0; i<dpIndex; i++) {
		intPart.insert(intPart.begin(),fracPart[i]);
	}
	return intPart;
}

int BinConverter::binToDecInt(std::vector<bool> bin) {
	int result = 0;
	for(unsigned int i=0; i<bin.size(); i++) {
		result += pow(2, i) * bin[i];
	}
	return result;
}

float BinConverter::binToDecFloat(std::vector<bool> bin) {
	float result = 0;
	for(int i=-dpIndex; i<bits-dpIndex; i++) {
		result += pow(2, i) * bin[i+dpIndex];
	}
	return result;
}

