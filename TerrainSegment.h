#include "TerrainManager.h"/*
 * TerrainSegment.h
 *
 * Class for a section of terrain.
 * Can be specified to an abritrary number of quads, with an arbitrary quad size.
 *
 * Generates the terrain based on the height map generation algorithm.
 *
 *  Created on: May 28, 2012
 *      Author: Simon Davies
 */

#ifndef TERRAINSEGMENT_H_
#define TERRAINSEGMENT_H_

#include <GL/gl.h>
#include "Quad.h"
#include <vector>

class TerrainSegment {
private:
	std::vector <std::vector<Quad*> > quad_arrays;
	GLuint terrain_texture_id;

	int quad_count_width;
	int quad_count_height;

	Vector3D centre;

	bool drawing;

public:
	TerrainSegment(float x, float z, float segment_size, float quad_size,
			GLuint terrain_texture);
	virtual ~TerrainSegment();
	void init_quads();

	Vector3D get_centre();

	GLuint terrain_GL_obj;

	void set_draw(bool);
	bool get_draw();

private:
	Quad* calculate_new_quad(float x, float z, float size);
};

#endif /* TERRAINSEGMENT_H_ */
