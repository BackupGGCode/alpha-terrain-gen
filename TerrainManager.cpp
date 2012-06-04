/*
 * TerrainManager.cpp
 *
 *  Created on: Jun 3, 2012
 *      Author: Simon
 */

#include "TerrainManager.h"

#define SEGMENT_COUNT_WIDTH 9.0f
#define SEGMENT_COUNT_TOTAL SEGMENT_COUNT_WIDTH * SEGMENT_COUNT_WIDTH

// TODO: Limiting scope
// I can make things happen by having an array of TerrainSegments which only get
// initialized when required
// Might make the whole thing more manageable?

TerrainManager::TerrainManager(GLuint terrain_texture, GLfloat segment_size,
		ControllableCamera* camera){

	this->camera = camera;
	terrain_segments.resize(SEGMENT_COUNT_TOTAL);

	this->terrain_texture = terrain_texture;
	this->segment_size = segment_size;

	float start_x = camera->getPositions()->cam_pos.x - ((this->segment_size * SEGMENT_COUNT_WIDTH) / 2);
	float start_z = camera->getPositions()->cam_pos.z - ((this->segment_size * SEGMENT_COUNT_WIDTH) / 2);

	repopulate_terrain(start_x, start_z);
}

TerrainManager::~TerrainManager() {
	for(unsigned int i = 0; i < terrain_segments.size(); i++){
		delete(terrain_segments[i]);
	}
	terrain_segments.clear();
}

void TerrainManager::repopulate_terrain(float start_x, float start_z){
	// TODO:
	// This must be a value that will divide evenly into segment_size
	float quad_size = 1.5f;

	float x = start_x;
	float z = start_z;

	// Statically generate terrain segments
	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 9; j++){
			if(terrain_segments[(i * 9) + j] == NULL){
				terrain_segments[(i * 9) + j] = new TerrainSegment(x, z,
						segment_size, quad_size, terrain_texture);
			}
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

/** Checks for all distant segments that are no longer visible
 * and removes them from the game world
 */

// TODO: this is slow as balls and doesn't really help anyone
void TerrainManager::check_for_distant_segments(){
	for(unsigned int i = 0; i < terrain_segments.size(); i++){

		float distance = distance2d(terrain_segments[i]->get_centre(),
				camera->getPositions()->cam_pos);

		float target = segment_size * (SEGMENT_COUNT_WIDTH / 2);

//		printf("Distance %f - Target %f \n", distance, target);


		if(distance2d(terrain_segments[i]->get_centre(),
				camera->getPositions()->cam_pos) > segment_size * (SEGMENT_COUNT_WIDTH)){
			delete terrain_segments[i];
			terrain_segments[i] = NULL;
			printf("KILL!\n");
		}
	}
	float start_x = camera->getPositions()->cam_pos.x - ((this->segment_size * SEGMENT_COUNT_WIDTH));
	float start_z = camera->getPositions()->cam_pos.z - ((this->segment_size * SEGMENT_COUNT_WIDTH));

	repopulate_terrain(start_x, start_z);
}


void TerrainManager::draw(){
	// Draw Terrain
	for(unsigned int i = 0; i < terrain_segments.size(); i++){
		glCallList(terrain_segments[i]->terrain_GL_obj);
	}
}

void TerrainManager::reset(){
	for(unsigned int i = 0; i < terrain_segments.size(); i++){
		delete terrain_segments[i];
		terrain_segments[i] = NULL;
	}
	float start_x = camera->getPositions()->cam_pos.x - ((this->segment_size * SEGMENT_COUNT_WIDTH));
	float start_z = camera->getPositions()->cam_pos.z - ((this->segment_size * SEGMENT_COUNT_WIDTH));

	repopulate_terrain(start_x, start_z);
}


// TODO: How do I do this?
//void TerrainManager::cull_distant_terrain_segments(){
//	for(unsigned int i = 0; i < terrain_segments.size(); i++){
//		TerrainSegment* segment = terrain_segments[i];
//		if(distance2d(segment->get_centre(), camera->getPositions()->cam_pos) > fog_distance_end + (segment_size / 2)){
//			segment->set_draw(false);
//		}
//		else{
//			segment->set_draw(true);
//		}
//	}
//}

