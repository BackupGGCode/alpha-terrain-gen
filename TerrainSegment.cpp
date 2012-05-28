/*
 * TerrainSegment.cpp
 *
 *  Created on: May 28, 2012
 *      Author: Simon
 */

#include "TerrainSegment.h"
#include "heightgen.h"
#include <stdlib.h>
#include <stdio.h>

TerrainSegment::TerrainSegment(float x, float z, int width_quads, int height_quads, float quad_size) {
	quad_strip.resize(width_quads);
	for(unsigned int i = 0; i < quad_strip.size(); i++){
		quad_strip[i].resize(height_quads);
		for(unsigned int j = 0; j < quad_strip[i].size(); j++){
			quad_strip[i][j] = calculateNewQuad(x + (quad_size * i), z + (quad_size * j), quad_size);
		}
	}
	printf("CREATED\n");
}

TerrainSegment::~TerrainSegment() {
	for(unsigned int i = 0; i < quad_strip.size(); i++){
		for(unsigned int j = 0; j < quad_strip[i].size(); j++){
			delete(quad_strip)[i][j];
		}
	}
	quad_strip.clear();
	// TODO Auto-generated destructor stub
	printf("DELETED\n");
}

void TerrainSegment::init_quads(){
	glShadeModel(GL_FLAT);
	for(unsigned int i = 0; i < quad_strip.size(); i++){
		glBegin(GL_QUAD_STRIP);
		for(unsigned int j = 0; j < quad_strip[i].size(); j++){
			quad_strip[i][j]->init();
		}
		glEnd();
	}
}

#define TERRAIN_MULTIPLIER 6

Quad* TerrainSegment::calculateNewQuad(float x, float z, float size){
	GLfloat vector1[] = {x,brownianValue(x,z,3) * TERRAIN_MULTIPLIER, z};
	GLfloat vector2[] = {x,brownianValue(x,z + size,3) * TERRAIN_MULTIPLIER, z + size};
	GLfloat vector3[] = {x + size, brownianValue(x + size,z,3) * TERRAIN_MULTIPLIER, z};
	GLfloat vector4[] = {x + size, brownianValue(x + size,z + size,3)  * TERRAIN_MULTIPLIER, z + size};

	Quad *quad = new Quad(vector1, vector2, vector3, vector4);
	return quad;
}
