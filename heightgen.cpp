/*
 * HeightGen.cpp
 *
 * HeightMap generation.
 *
 * Uses Fractional Brownian Motion algorithm to generate
 * values that can be used for a heightmap.
 */


#include <stdio.h>
#include <stdlib.h>

#include <math.h>

#include "DetRand.h"
#include "HeightGen.h"

#define PI 3.1415926

/** Smoothes a single point based on the average of its neighbors */
static float getSmoothValue(int x, int y){
	float cornersValue = (getRandFloat(x-1,y-1) + getRandFloat(x+1,y-1)
			+ getRandFloat(x-1,y+1) + getRandFloat(x+1,y+1)) / 16;
	float sides = (getRandFloat(x-1,y) + getRandFloat(x+1,y)
			+ getRandFloat(x,y-1) + getRandFloat(x,y+1)) / 8;
	float value = getRandFloat(x,y) / 4;

	return cornersValue + sides + value;
}

/** Performs a cosine interpolation between two values */
static double interpolate(double a, double b, double x){
	double ft = x * PI;
	double f = (1 - cos(ft)) * 0.5;

	return a * (1 - f) + b * f;
}

static float interpolatedNoiseValue(float x, float y){
	// TODO: Currently just getting the absolute value
	// Makes it symmetrical, but fixes the bug I was having
	int integer_X = (int)floor(x);
	float fraction_X = x - integer_X;

	int integer_Y = (int)floor(y);
	float fraction_Y = y - integer_Y;

	float v1 = getSmoothValue(integer_X, integer_Y);
	float v2 = getSmoothValue(integer_X + 1, integer_Y);
	float v3 = getSmoothValue(integer_X, integer_Y + 1);
	float v4 = getSmoothValue(integer_X + 1, integer_Y + 1);

	float i1 = (float)interpolate(v1, v2, fraction_X);
	float i2 = (float)interpolate(v3, v4, fraction_X);

	return (float)interpolate(i1,i2, fraction_Y);
}

/* Get the Fractional Brownian Motion value for a given point */
float brownianValue(float x, float y, int octavesCount){

	float gain = 0.525f;
	float lacunarity = 2.0f;

	float total = 0;
	float frequency = 0.25f;
	float amplitude = gain;

	for(int i = 0; i < octavesCount; i++){
		total += interpolatedNoiseValue(x * frequency ,y * frequency) * amplitude;
		frequency *= lacunarity;
		amplitude *= gain;
	}

	return total;
}
