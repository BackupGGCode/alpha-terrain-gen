/*
 * TerrainManager.h
 *
 *  Created on: Jun 3, 2012
 *      Author: Simon
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
	// Terrain settings

	float segment_size;

	GLuint terrain_texture;

	// Camera required for determining what terrain segments
	// should be in memory at any givent ime
	ControllableCamera* camera;

	// Terrain Vectors
	std::vector<TerrainSegment*> terrain_segments;

	void repopulate_terrain(float start_x, float start_z);
public:
	TerrainManager(GLuint terrain_texture, GLfloat segment_size,
			ControllableCamera* camera);
	virtual ~TerrainManager();
	void draw();
	void reset();
	// TODO: Terrain culling / updating
	void check_for_distant_segments();
};

#endif /* TERRAINMANAGER_H_ */
