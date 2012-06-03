/*
 * TerrainManager.h
 *
 *  Created on: Jun 3, 2012
 *      Author: Simon
 */

#ifndef TERRAINMANAGER_H_
#define TERRAINMANAGER_H_

#include "TerrainSegment.h"
#include <GL/gl.h>
#include <vector>

class TerrainManager {
private:
	// Terrain settings
	float quad_size;
	float segment_size;

	// Terrain Vectors
	std::vector<TerrainSegment*> terrain_segments;

	// TODO: Terrain culling / updating
	void cull_distant_terrain_segments();
public:
	TerrainManager(GLuint terrain_texture);
	virtual ~TerrainManager();
	void draw();
};

#endif /* TERRAINMANAGER_H_ */
