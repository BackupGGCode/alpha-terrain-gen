/*
 * TerrainSegment.h
 *
 *  Created on: May 28, 2012
 *      Author: Simon
 */

#ifndef TERRAINSEGMENT_H_
#define TERRAINSEGMENT_H_

#include <GL/gl.h>
#include "Quad.h"
#include <vector>

class TerrainSegment {
private:
	std::vector <std::vector<Quad*> > quad_strip;

public:
	TerrainSegment(float x, float z, int width_quads, int height_quads, float quad_size);
	virtual ~TerrainSegment();
	void init_quads();

private:
	Quad* calculateNewQuad(float x, float z, float size);
};

#endif /* TERRAINSEGMENT_H_ */
