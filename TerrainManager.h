/*
 * TerrainManager.h
 *
 * Manager for the Terrain.
 *
 * Dynamically generates new terrain segments and removes existing distant
 * segments.
 *
 *  Created on: Jun 3, 2012
 *      Author: Simon Davies
 */

#ifndef TERRAINMANAGER_H_
#define TERRAINMANAGER_H_

#include "TerrainSegment.h"
#include "Vector3D.h"
#include "ControllableCamera.h"
#include <GL/gl.h>
#include <vector>

class TerrainManager {
private:

	// These values hold the real world
	// x and z coordinates of where the position of
	// the array entry 0,0.
	float array_start_value;

	// World size in segments
	// This is the width / height of a square world
	unsigned int world_width;

	// Terrain settings
	float segment_size;

	GLuint terrain_texture;

	// Camera required for determining what terrain segments
	// should be in memory at any givent ime
	ControllableCamera* camera;

	// Terrain array
	TerrainSegment** terrain_segments;

	// Draw Table
	// Used to determine what is being drawn,
	// and which segments should be removed from memory
	bool* draw_table;

	float translate_to_index_coordinates(float x);
	float get_nearest_segment_start(float x);

	void erase_distant_segments();

public:
	TerrainManager(GLuint terrain_texture, GLfloat segment_size,
			ControllableCamera* camera, unsigned int world_width);
	virtual ~TerrainManager();
	void draw();
	void repopulate_terrain();
	void initialize_new_terrain_segments();
};

#endif /* TERRAINMANAGER_H_ */
