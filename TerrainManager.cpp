/*
 * TerrainManager.cpp
 *
 *  Created on: Jun 3, 2012
 *      Author: Simon
 */

#include "TerrainManager.h"

#include "Vector3D.h"

TerrainManager::TerrainManager(GLuint terrain_texture){
	terrain_segments.resize(81);
	float start_x = -240;
	float start_z = -240;

	// This must be a value that will divide evenly into segment_size
	quad_size = 1.5f;
	segment_size = 60.0f;

	float x = start_x;
	float z = start_z;

	// Statically generate terrain segments
	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 9; j++){
			terrain_segments[(i * 9) + j] = new TerrainSegment(x, z,
					segment_size, quad_size, terrain_texture);
			z += segment_size;
		}
		z = start_z;
		x += segment_size;
	}

	/* Initialise terrain segments */
	for(unsigned int i = 0; i < terrain_segments.size(); i++){
		terrain_segments[i]->terrain_GL_obj = glGenLists(1);

		glNewList(terrain_segments[i]->terrain_GL_obj, GL_COMPILE);

		terrain_segments[i]->init_quads();
		glEndList();
	}
}

TerrainManager::~TerrainManager() {
	for(unsigned int i = 0; i < terrain_segments.size(); i++){
		delete(terrain_segments[i]);
	}
	terrain_segments.clear();
}

void TerrainManager::draw(){
	// Draw Terrain
	for(unsigned int i = 0; i < terrain_segments.size(); i++){
		if(terrain_segments[i]->get_draw()){
			glCallList(terrain_segments[i]->terrain_GL_obj);
		}
	}
}


// TODO: How do I do this?
void TerrainManager::cull_distant_terrain_segments(){
	for(unsigned int i = 0; i < terrain_segments.size(); i++){
		TerrainSegment* segment = terrain_segments[i];
		if(distance2d(segment->get_centre(), camera->getPositions()->cam_pos) > fog_distance_end + (segment_size / 2)){
			segment->set_draw(false);
		}
		else{
			segment->set_draw(true);
		}
	}
}

