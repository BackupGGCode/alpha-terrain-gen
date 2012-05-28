/*
 *
 * heightgen.h
 *
 * HeightMap generation.
 *
 * Uses Fractional Brownian Motion algorithm to generate
 * values that can be used for a heightmap.
 *
 *  Created on: May 24, 2012
 *      Author: Simon Davies
 */

#ifndef HEIGHTGEN_H_
#define HEIGHTGEN_H_

float brownianValue(float x, float y, int octavesCount);

#endif /* HEIGHTGEN_H_ */
