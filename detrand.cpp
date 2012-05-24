/*
 * detrand.cpp
 *
 *  Created on: May 24, 2012
 *      Author: Simon Davies
 */

#include <stdio.h>
#include <stdlib.h>

#include "detrand.h"

#define START_SEED 4317903

float getFloat(int x){
	srand(x * START_SEED);
	return (float)rand() / RAND_MAX;
}

float getFloat(int x, int y){
	srand(x * START_SEED + (y * START_SEED % (x + x + 1)));
	return (float)rand() / RAND_MAX;
}
