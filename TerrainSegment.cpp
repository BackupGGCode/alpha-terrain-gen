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
	// Creates the two dimensiona array full of quads
	quad_arrays.resize(width_quads);
	for(unsigned int i = 0; i < quad_arrays.size(); i++){
		quad_arrays[i].resize(height_quads);
		for(unsigned int j = 0; j < quad_arrays[i].size(); j++){
			quad_arrays[i][j] = calculateNewQuad(x + (quad_size * i), z + (quad_size * j), quad_size);
		}
	}
	// TODO: Calculate vertex normals for each vertex
	// TODO: Calculate the edges
//	for(unsigned int i = 1; i < quad_arrays.size() - 1; i++){
//		for(unsigned int j = 1; j < quad_arrays[i].size() -1; j++){
//			quad_arrays[i][j]->calculate_vertex_normals(
//					quad_arrays[i-1][j-1], quad_arrays[i][j-1], quad_arrays[i+1][j-1],
//					quad_arrays[i-1][j], /* This Quad */	    quad_arrays[i+1][j],
//					quad_arrays[i-1][j+1], quad_arrays[i][j+1], quad_arrays[i+1][j+1]
//					);
//		}
//	}
}

TerrainSegment::~TerrainSegment() {
	for(unsigned int i = 0; i < quad_arrays.size(); i++){
		for(unsigned int j = 0; j < quad_arrays[i].size(); j++){
			delete(quad_arrays)[i][j];
		}
	}
	quad_arrays.clear();
}

/** Initializes all of the quads for rendering by OpenGL */
void TerrainSegment::init_quads(){
	glShadeModel(GL_SMOOTH);
	glBegin(GL_QUADS);
	for(unsigned int i = 0; i < quad_arrays.size(); i++){
		for(unsigned int j = 0; j < quad_arrays[i].size(); j++){
			// TODO: Test colours
//			glColor3f((float)i / quad_arrays.size(),(float)j / quad_arrays.size(),0.5);
			quad_arrays[i][j]->init();
		}
	}
	glEnd();
}

#define TERRAIN_MULTIPLIER 6

/** Calculates a new quad given the x and z coordinate, and the size of the quad. */
Quad* TerrainSegment::calculateNewQuad(float x, float z, float size){
	Vector3d* vector1 = new Vector3d(x,brownianValue(x,z,3) * TERRAIN_MULTIPLIER, z);
	Vector3d* vector2 = new Vector3d(x,brownianValue(x,z + size,3) * TERRAIN_MULTIPLIER, z + size);
	Vector3d* vector3 = new Vector3d(x + size, brownianValue(x + size,z + size,3)  * TERRAIN_MULTIPLIER, z + size);
	Vector3d* vector4 = new Vector3d(x + size, brownianValue(x + size,z,3) * TERRAIN_MULTIPLIER, z);

	Quad *quad = new Quad(vector1, vector2, vector3, vector4);
	return quad;
}
