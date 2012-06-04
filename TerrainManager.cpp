/*
 * TerrainManager.cpp
 *
 * Manager for the Terrain.
 *
 * Dynamically generates new terrain segments and removes existing distant
 * segments.
 *
 *  Created on: Jun 3, 2012
 *      Author: Simon Davies
 */

#include "TerrainManager.h"
#include <Math.h>
#include "SDL.h"
#include "SDL_thread.h"

// How many segments (width) do we want to try and draw?
#define SEGMENT_DRAW_COUNT 16

// Semaphore for the draw table
SDL_sem *draw_table_lock = NULL;

/** Constructor for terrain managers
 * ----------------------------------------------------------------------------
 * Arguments:
 * GLuint terrain_texture - ID for the texture for the terrain
 * GLfloat segment_size - Size of the terrain segment in real-size coordinates
 * ControllableCamera* camera - The camera within the world, used to determine which
 * 								terrain segments should be in memory.
 * 	unsigned int world_width  - The width of the world in terrain segments.
 * 								The world is assumed to be square, so this value is also
 * 								used as the height of the world.
 */
TerrainManager::TerrainManager(GLuint terrain_texture, GLfloat segment_size,
		ControllableCamera* camera, unsigned int world_width) {

	// Set values locally
	this->world_width = world_width;
	this->camera = camera;

	// Initialize array
	terrain_segments = new TerrainSegment*[world_width * world_width];

	// Draw Table
	draw_table = new bool[world_width * world_width];
	draw_table_lock = SDL_CreateSemaphore(1);

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
}

TerrainManager::~TerrainManager() {
	for (unsigned int i = 0; i < world_width; i++) {
		for (unsigned int j = 0; j < world_width; j++) {
			delete (terrain_segments[i * world_width + j]);
		}
	}
	delete (terrain_segments);
}

/** Takes a value and returns the coordinate that would be correct for starting a
 * new terrain segment.
 * e.g. removes the remainder from dividing by the segment size.
 * Arguments
 * -----------------------------------------------
 * float x - input value
 */
float TerrainManager::get_nearest_segment_start(float x) {
	return x - (fmod(x, segment_size));
}

/** Repopulates the terrain around the camera location */
void TerrainManager::repopulate_terrain() {
	float start_x = get_nearest_segment_start(
			camera->getPositions()->cam_pos.x
					- ((this->segment_size * SEGMENT_DRAW_COUNT) / 2));
	float start_z = get_nearest_segment_start(
			camera->getPositions()->cam_pos.z
					- ((this->segment_size * SEGMENT_DRAW_COUNT) / 2));
	// This must be a value that will divide evenly into segment_size
	float quad_size = 3.0f;

	float x = start_x;
	float z = start_z;

	int start_i = (int) translate_to_index_coordinates(x);
	int start_j = (int) translate_to_index_coordinates(z);

	// Statically generate terrain segments
	// TODO: Check for the boundaries of the world array
	for (int i = start_i; i < start_i + SEGMENT_DRAW_COUNT; i++) {
		for (int j = start_j; j < start_j + SEGMENT_DRAW_COUNT; j++) {
			if (terrain_segments[(i * world_width) + j] == NULL) {
				SDL_WM_SetCaption("Alpha - Loading new Terrain...", "alpha");
				terrain_segments[(i * world_width) + j] = new TerrainSegment(x,
						z, segment_size, quad_size, terrain_texture);
			}
			z += segment_size;
		}
		z = start_z;
		x += segment_size;
	}
	erase_distant_segments();
	SDL_WM_SetCaption("Alpha", "alpha");
}

/** Initializes and compiles any uninitialized terrain segments
 * for use by OpenGL.
 */
void TerrainManager::initialize_new_terrain_segments() {
	float start_x = get_nearest_segment_start(
			camera->getPositions()->cam_pos.x
					- ((this->segment_size * SEGMENT_DRAW_COUNT) / 2));
	float start_z = get_nearest_segment_start(
			camera->getPositions()->cam_pos.z
					- ((this->segment_size * SEGMENT_DRAW_COUNT) / 2));

	float x = start_x;
	float z = start_z;

	unsigned int start_i = (int) translate_to_index_coordinates(x);
	unsigned int start_j = (int) translate_to_index_coordinates(z);

	// Lock the draw table
	SDL_SemWait(draw_table_lock);
	// The draw table stores information on which segments are still being
	// actively drawn by the program.
	// Anything that is set to false by the time the semaphore is released
	// can be removed from memory
	for(unsigned int i = 0; i < world_width * world_width; i++){
		draw_table[i] = false;
	}

	/* Initialise terrain segments */
	for (unsigned int i = start_i; i < start_i + SEGMENT_DRAW_COUNT; i++) {
		for (unsigned int j = start_j; j < start_j + SEGMENT_DRAW_COUNT; j++) {
			// Update the draw table, so that this information to inform the
			// garbage collection that this segment is being drawn
			draw_table[(i * world_width) + j] = true;
			TerrainSegment* current_segment = terrain_segments[(i * world_width)
					+ j];
			if (current_segment != NULL
					&& !current_segment->get_initialized()) {
				current_segment->terrain_GL_obj = glGenLists(1);
				glNewList(current_segment->terrain_GL_obj, GL_COMPILE);
				current_segment->init_quads();
				glEndList();
			}
		}
	}
	// Unlock the draw table
	SDL_SemPost(draw_table_lock);
}

/** Erases any distant segment in memory.
 * This is determined by looking at which segments are set as
 * false in the draw table.
 */
void TerrainManager::erase_distant_segments() {
	SDL_SemWait(draw_table_lock);
	for (unsigned int i = 0; i < world_width; i++) {
		for (unsigned int j = 0; j < world_width; j++) {
			// If the draw table entry is false,
			// then delete the terrain segment
			if(!draw_table[(i * world_width) + j]){
				delete(terrain_segments[(i * world_width) + j]);
				terrain_segments[(i * world_width) + j] = NULL;
			}
		}
	}
	SDL_SemPost(draw_table_lock);
}

/** Turns a real world coordinate into the index into the
 * terrain_segments array. Based on the size of the terrain segments.
 *
 * Arguments
 * -------------------------------------------------------
 * float x : input coordinate value
 */
float TerrainManager::translate_to_index_coordinates(float x) {
	x -= array_start_value;
	x /= segment_size;
	return x;
}

/**
 * Draws the terrain.
 * Assumes all of the terrain segments that should be drawn have
 * already been initialized.
 */
void TerrainManager::draw() {
	float start_x = get_nearest_segment_start(
			camera->getPositions()->cam_pos.x
					- ((segment_size * SEGMENT_DRAW_COUNT) / 2));
	float start_z = get_nearest_segment_start(
			camera->getPositions()->cam_pos.z
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
