/*
 * Utils.cpp
 *
 *  Created on: 26 Jul 2015
 *      Author: Jack
 */

#include "Utils.h"

std::vector<bool> splice(std::vector<bool> v, int start, int end) {
	int newLen = end-start;
	std::vector<bool> newV(newLen);

	for(int i=0; i<newLen; i++) {
		newV[i] = v[start+i];
	}

	return newV;
}


