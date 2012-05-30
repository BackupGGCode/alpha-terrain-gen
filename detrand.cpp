/*
 * detrand.cpp
 *
 * Deterministic Random Number Generator
 *
 *  Created on: May 24, 2012
 *      Author: Simon Davies
 */

#include <stdio.h>
#include <stdlib.h>

#include "DetRand.h"

#define START_SEED 4317903

/** Gets a random float, using a single value x as its seed */
float getRandFloat(int x){
	if(x == 0){
		x = 3241;
	}
	srand(x * START_SEED);
	return (float)rand() / RAND_MAX;
}

/** Gets a random float, using a two values, x and y, as its seed */
float getRandFloat(int x, int y){
	if(x == 0){
		x = 3241;
	}
	if(y == 0){
		y = 3241;
	}
	srand(START_SEED * x * y);
	return (float)rand() / RAND_MAX;
}
