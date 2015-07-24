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

	bool positive = true;
	int currentDec = dec;
	if(dec < 0) {
		//Add a sign bit
		positive = false;
		currentDec *= -1;
	} else {
		positive = true;
	}

	while(currentDec != 0) {
		result.push_back(currentDec % 2);
		//std::cout << currentDec << std::endl;
		currentDec /= 2;
	}
	if(result.size() < bits-1) {
		unsigned int numZeroes = bits-result.size()-1;
		for(unsigned int i=0; i<numZeroes; i++) {
			//std::cout << i << ": ";
			result.push_back(0);
			//printBinString(result);
			//std::cout << "bits-size = " << bits-result.size()-1 << std::endl;
		}
	}
	if(positive) {
		result.push_back(0);
	}
	else {
		result.push_back(1);
	}
	return result;
}

std::vector<bool> BinConverter::decToBin(float dec) {
	int decInt;
	if(dec < 0) {
		decInt = (int)ceil(dec);
	} else {
		decInt = (int)floor(dec);
	}

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
	for(unsigned int i=0; i<bin.size()-1; i++) {
		result += pow(2, i) * bin[i];
	}
	if(*bin.end() == 1) {
		result *= -1;
	}
	return result;
}

float BinConverter::binToDecFloat(std::vector<bool> bin) {
	float result = 0;
	for(int i=-dpIndex; i<bits-dpIndex-1; i++) {
		result += pow(2, i) * bin[i+dpIndex];
	}
	if(*bin.end() == 1) {
		result *= -1;
	}
	return result;
}

void printBinString(std::vector<bool> bin) {
	for(int i=0; i<bin.size(); i++) {
		std::cout << bin[i];
	}
	std::cout << std::endl;
}

