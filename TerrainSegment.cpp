/*
 * TerrainSegment.cpp
 *
 * Class for a section of terrain.
 * Can be specified to an abritrary number of quads, with an arbitrary quad size.
 *
 * Generates the terrain based on the height map generation algorithm.
 *
 *  Created on: May 28, 2012
 *      Author: Simon Davies
 */

#include "TerrainSegment.h"
#include "heightgen.h"
#include <stdlib.h>
#include <stdio.h>

TerrainSegment::TerrainSegment(float x, float z, int width_quads, int height_quads, float quad_size) {
	quad_arrays.resize(width_quads);
	for(unsigned int i = 0; i < quad_arrays.size(); i++){
		quad_arrays[i].resize(height_quads);
		for(unsigned int j = 0; j < quad_arrays[i].size(); j++){
			quad_arrays[i][j] = calculateNewQuad(x + (quad_size * i), z + (quad_size * j), quad_size);
		}
	}
	printf("CREATED\n");
}

TerrainSegment::~TerrainSegment() {
	for(unsigned int i = 0; i < quad_arrays.size(); i++){
		for(unsigned int j = 0; j < quad_arrays[i].size(); j++){
			delete(quad_arrays)[i][j];
		}
	}
	quad_arrays.clear();
	// TODO Auto-generated destructor stub
	printf("DELETED\n");
}

/** Initializes all of the quads for rendering by OpenGL */
void TerrainSegment::init_quads(){
	glShadeModel(GL_SMOOTH);
	glBegin(GL_QUADS);
	for(unsigned int i = 0; i < quad_arrays.size(); i++){
		for(unsigned int j = 0; j < quad_arrays[i].size(); j++){
			quad_arrays[i][j]->init();
		}
	}
	glEnd();
}

#define TERRAIN_MULTIPLIER 6

/** Calculates a new quad given the x and z coordinate, and the size of the quad. */
Quad* TerrainSegment::calculateNewQuad(float x, float z, float size){
	GLfloat vector1[] = {x,brownianValue(x,z,3) * TERRAIN_MULTIPLIER, z};
	GLfloat vector2[] = {x,brownianValue(x,z + size,3) * TERRAIN_MULTIPLIER, z + size};
	GLfloat vector3[] = {x + size, brownianValue(x + size,z + size,3)  * TERRAIN_MULTIPLIER, z + size};
	GLfloat vector4[] = {x + size, brownianValue(x + size,z,3) * TERRAIN_MULTIPLIER, z};

	Quad *quad = new Quad(vector1, vector2, vector3, vector4);
	return quad;
}
