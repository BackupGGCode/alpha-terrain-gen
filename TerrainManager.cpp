/*
 * TerrainManager.cpp
 *
 *  Created on: Jun 3, 2012
 *      Author: Simon
 */

#include "TerrainManager.h"
#include <Math.h>

#define SEGMENT_DRAW_COUNT 9

// TODO: Limiting scope
// I can make things happen by having an array of TerrainSegments which only get
// initialized when required
// Might make the whole thing more manageable?

TerrainManager::TerrainManager(GLuint terrain_texture, GLfloat segment_size,
		ControllableCamera* camera, unsigned int world_width) {

	this->world_width = world_width;
	this->camera = camera;
	terrain_segments = new TerrainSegment*[world_width * world_width];

	// Set all segments to NULL
	for (unsigned int i = 0; i < world_width; i++) {
		for (unsigned int j = 0; j < world_width; j++) {
			terrain_segments[i * world_width + j] = NULL;
		}
	}

	// Set other variables
	// Terrain texture bitmap ID
	this->terrain_texture = terrain_texture;

	// Size of each segment variable
	this->segment_size = segment_size;

	array_start_value = 0 - (world_width / 2 * segment_size);

	float start_x = get_nearest_segment_start(camera->getPositions()->cam_pos.x
			- ((this->segment_size * SEGMENT_DRAW_COUNT) / 2));
	float start_z = get_nearest_segment_start(camera->getPositions()->cam_pos.z
			- ((this->segment_size * SEGMENT_DRAW_COUNT) / 2));

	repopulate_terrain(start_x, start_z);
}

TerrainManager::~TerrainManager() {
	for (unsigned int i = 0; i < world_width; i++) {
		for (unsigned int j = 0; j < world_width; j++) {
			delete (terrain_segments[i * world_width + j]);
		}
	}
	delete (terrain_segments);
}

float TerrainManager::get_nearest_segment_start(float x){
	return x - (fmod(x, segment_size));
}

/** Repopulates the terrain around the camera location */
void TerrainManager::repopulate_terrain(float start_x, float start_z) {
	// This must be a value that will divide evenly into segment_size
	float quad_size = 3.0f;

	float x = start_x;
	float z = start_z;

	int start_i = (int) translate_to_index_coordinates(x);
	int start_j = (int) translate_to_index_coordinates(z);

	// Statically generate terrain segments
	// TODO: Change the magic numbers
	for (int i = start_i; i < start_i + SEGMENT_DRAW_COUNT; i++) {
		for (int j = start_j; j < start_j + SEGMENT_DRAW_COUNT; j++) {
			if (terrain_segments[(i * world_width) + j] == NULL) {
				terrain_segments[(i * world_width) + j] = new TerrainSegment(x,
						z, segment_size, quad_size, terrain_texture);
			}
			z += segment_size;
		}
		z = start_z;
		x += segment_size;
	}

	/* Initialise terrain segments */
	for (int i = start_i; i < start_i + SEGMENT_DRAW_COUNT; i++) {
		for (int j = start_j; j < start_j + SEGMENT_DRAW_COUNT; j++) {
			TerrainSegment* current_segment = terrain_segments[(i * world_width)
					+ j];

			current_segment->terrain_GL_obj = glGenLists(1);
			glNewList(current_segment->terrain_GL_obj, GL_COMPILE);
			current_segment->init_quads();
			glEndList();
		}
	}
}

void TerrainManager::reset() {
	float start_x = get_nearest_segment_start(camera->getPositions()->cam_pos.x
			- ((segment_size * SEGMENT_DRAW_COUNT) / 2));
	float start_z = get_nearest_segment_start(camera->getPositions()->cam_pos.z
			- ((segment_size * SEGMENT_DRAW_COUNT) / 2));

	repopulate_terrain(start_x, start_z);
}

float TerrainManager::translate_to_index_coordinates(float x) {
	x -= array_start_value;
	x /= segment_size;
	return x;
}

void TerrainManager::draw() {
	float start_x = get_nearest_segment_start(camera->getPositions()->cam_pos.x
			- ((segment_size * SEGMENT_DRAW_COUNT) / 2));
	float start_z = get_nearest_segment_start(camera->getPositions()->cam_pos.z
			- ((segment_size * SEGMENT_DRAW_COUNT) / 2));

	int start_i = (int) translate_to_index_coordinates(start_x);
	int start_j = (int) translate_to_index_coordinates(start_z);
	// Draw Terrain
	for (int i = start_i; i < start_i + SEGMENT_DRAW_COUNT; i++) {
		for (int j = start_j; j < start_j + SEGMENT_DRAW_COUNT; j++) {
			TerrainSegment* current_segment = terrain_segments[(i * world_width)
					+ j];
			if (current_segment != NULL) {
				glCallList(current_segment->terrain_GL_obj);
			}
		}
	}
}

