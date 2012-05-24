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

float getRandFloat(int x){
	if(x == 0){
		x = 3241;
	}
	srand(x * START_SEED);
	return (float)rand() / RAND_MAX;
}

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
