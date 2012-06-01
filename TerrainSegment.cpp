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
#include "Heightgen.h"
#include "Vector3D.h"
#include <stdlib.h>
#include <stdio.h>

// TODO: Should work with 2, 4 temp fix
#define EXTRA_QUADS_FOR_VERTEX_NORMAL_CALC 4

TerrainSegment::TerrainSegment(float x, float z, float segment_size, float quad_size,
		GLuint terrain_texture,
		GLuint grass_sprite_texture) {
	this->terrain_texture_id = terrain_texture;
	this->grass_sprite_texture_id = grass_sprite_texture;

	quad_count_width = (int)(segment_size / quad_size);
	quad_count_height = (int)(segment_size / quad_size);

	// Creates the two dimensional array full of quads
	// The array size is increased so that the extra quads are used for
	// calculating vertex normals on the edge quads.

	// These extra quads are not drawn.
	quad_arrays.resize(quad_count_width + EXTRA_QUADS_FOR_VERTEX_NORMAL_CALC);
	for(unsigned int i = 0; i < quad_arrays.size(); i++){
		quad_arrays[i].resize(quad_count_height + EXTRA_QUADS_FOR_VERTEX_NORMAL_CALC);
		for(unsigned int j = 0; j < quad_arrays[i].size(); j++){
			// - 1 is taken from the i and j index values so that the imaginary quads are rendered outside of the specified
			// location
			quad_arrays[i][j] = calculateNewQuad(x + (quad_size * (i - 1)), z + (quad_size * (j - 1)), quad_size);
		}
	}

	// Calculate vertex normals for each vertex
	for(unsigned int i = EXTRA_QUADS_FOR_VERTEX_NORMAL_CALC / 2; i < quad_arrays.size() - EXTRA_QUADS_FOR_VERTEX_NORMAL_CALC / 2; i++){
		for(unsigned int j = EXTRA_QUADS_FOR_VERTEX_NORMAL_CALC / 2; j < quad_arrays[i].size() - EXTRA_QUADS_FOR_VERTEX_NORMAL_CALC / 2; j++){
			quad_arrays[i][j]->calculate_vertex_normals(
					quad_arrays[i-1][j-1], quad_arrays[i][j-1], quad_arrays[i+1][j-1],
					quad_arrays[i-1][j], /* This Quad */	    quad_arrays[i+1][j],
					quad_arrays[i-1][j+1], quad_arrays[i][j+1], quad_arrays[i+1][j+1]
					);
			quad_arrays[i][j]->calculate_materials();
		}
	}


#ifdef GRASS_SPRITES
	// Generate grass sprites
	for(unsigned int i = EXTRA_QUADS_FOR_VERTEX_NORMAL_CALC / 2; i < quad_arrays.size() - EXTRA_QUADS_FOR_VERTEX_NORMAL_CALC / 2; i++){
		for(unsigned int j = EXTRA_QUADS_FOR_VERTEX_NORMAL_CALC / 2; j < quad_arrays[i].size() - EXTRA_QUADS_FOR_VERTEX_NORMAL_CALC / 2; j++){
			quad_arrays[i][j]->generate_grass_sprites();
		}
	}
#endif
	// Set the centre vector for the segment
	centre = Vector3D(x + segment_size / 2, 0.0, z + segment_size / 2);
}

TerrainSegment::~TerrainSegment() {
	for(unsigned int i = 0; i < quad_arrays.size(); i++){
		for(unsigned int j = 0; j < quad_arrays[i].size(); j++){
			delete quad_arrays[i][j];
		}
	}
	quad_arrays.clear();
}

#define TEX_LOOP_PER_SEGMENT 2

/** Initializes all of the quads for rendering by OpenGL */
void TerrainSegment::init_quads(){
	glShadeModel(GL_SMOOTH);
	glBindTexture (GL_TEXTURE_2D, terrain_texture_id);

	glBegin(GL_QUADS);

	float tex_x = 0.0;
	float tex_y = 0.0;

	float tex_size = (1.0 / quad_count_height) * TEX_LOOP_PER_SEGMENT;

	// TODO: 2 should be 1 if I can fix the edge normals problem
	for(unsigned int i = EXTRA_QUADS_FOR_VERTEX_NORMAL_CALC / 2; i < quad_arrays.size() - EXTRA_QUADS_FOR_VERTEX_NORMAL_CALC / 2; i++){
		for(unsigned int j = EXTRA_QUADS_FOR_VERTEX_NORMAL_CALC / 2; j < quad_arrays[i].size() - EXTRA_QUADS_FOR_VERTEX_NORMAL_CALC / 2; j++){
			quad_arrays[i][j]->init(terrain_texture_id, tex_x, tex_y, tex_size);
			tex_x += tex_size;
		}
		tex_x = 0;
		tex_y += tex_size;
	}
	glEnd();

	// Transparent grass textures

	// Disable updating the occlusion data
#ifdef GRASS_SPRITES
	glDepthMask(GL_FALSE);
	glDisable(GL_CULL_FACE);
	// Enable blending
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glShadeModel(GL_SMOOTH);

	glBindTexture (GL_TEXTURE_2D, grass_sprite_texture_id);

	glBegin(GL_QUADS);

	for(unsigned int i = EXTRA_QUADS_FOR_VERTEX_NORMAL_CALC / 2; i < quad_arrays.size() - EXTRA_QUADS_FOR_VERTEX_NORMAL_CALC / 2; i++){
		for(unsigned int j = EXTRA_QUADS_FOR_VERTEX_NORMAL_CALC / 2; j < quad_arrays[i].size() - EXTRA_QUADS_FOR_VERTEX_NORMAL_CALC / 2; j++){
			quad_arrays[i][j]->init_grass();
		}
	}

	glEnd();

	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glDepthMask(GL_TRUE);
#endif
}

#define TERRAIN_MULTIPLIER 10

/** Calculates a new quad given the x and z coordinate, and the size of the quad. */
Quad* TerrainSegment::calculateNewQuad(float x, float z, float size){
	Vector3D* vector1 = new Vector3D(x,brownianValue(x,z,3) * TERRAIN_MULTIPLIER, z);
	Vector3D* vector2 = new Vector3D(x,brownianValue(x,z + size,3) * TERRAIN_MULTIPLIER, z + size);
	Vector3D* vector3 = new Vector3D(x + size, brownianValue(x + size,z + size,3)  * TERRAIN_MULTIPLIER, z + size);
	Vector3D* vector4 = new Vector3D(x + size, brownianValue(x + size,z,3) * TERRAIN_MULTIPLIER, z);

	Quad *quad = new Quad(vector1, vector2, vector3, vector4);
	return quad;
}
