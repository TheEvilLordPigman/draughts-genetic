/*
 * main.cpp
 *
 *  Created on: 12 Jun 2015
 *      Author: Jack
 */

#include "DraughtsAI.h"
#include "Population.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <random>

int main(int argc, char* args[]) {
	srand(time(NULL));
	Population pop;
	pop.assignFitness();
	pop.printPop();
	while(true) {
		pop.selectParents();
		pop.breedNextGen();
		pop.assignFitness();
		pop.printPop();
	}

}


